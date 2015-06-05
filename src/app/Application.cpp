#include <app/Application.h>
#include <serialization/DataParser.h>
#include <opengl/Shader.h>
#include <opengl/Use.h>
#include <opengl/Interface.h>
#include <opengl/BufferObject.h>
#include <opengl/VertexBuffer.h>
#include <opengl/Texture.h>
#include <utils/File.h>
#include <utils/Random.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_mouse.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>

using ce::Application;

namespace {
    float AttractorMovementSpeed;
}

Application::Application()
    :   running_( false ),
        window_(),
        context_(),
        camera_(),  //this will be moved somewhere else eventually
        compute_( nullptr ),
        display_( nullptr )
        {}

void Application::run() {
    initialize_();

    /*TEMPORARY TEST STUFF BEGINS HERE*/

    // compute particle simulator stuff
    const unsigned ParticleCount = ParticleGroupSize_ * ParticleGroupCount_;
    const unsigned MaxAttractors = 64;
	
    ce::VertexBuffer positionBuffer{};
    ce::BufferObject velocityBuffer{ GL_ARRAY_BUFFER };

    ce::Texture positionTbo( GL_TEXTURE_BUFFER );
    ce::Texture velocityTbo( GL_TEXTURE_BUFFER );

    GLuint renderVao;
    glGenVertexArrays( 1, &renderVao );
    glBindVertexArray( renderVao );

    positionBuffer.dataStore(
        ParticleCount * 4 * sizeof(float),
        NULL, GL_DYNAMIC_COPY
    );
    // the following operation binds positionBuffer to the buffer target
    float* posPtr = (float*) positionBuffer.mapBufferRange(
        0, ParticleCount * 4u * sizeof(float),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
    );
    for ( std::size_t i = 0u; i < 4u*ParticleCount; i += 4u ) {
        posPtr[i]       = ce::Randf( -10.0f, 10.0f );
        posPtr[i+1u]    = ce::Randf( -10.0f, 10.0f );
        posPtr[i+2u]    = ce::Randf( -10.0f, 10.0f );
        posPtr[i+3u]    = ce::Randf( 0.0f, 1.0f );
    }
    positionBuffer.unmapBuffer();   // and here we implicitly unbind it

    {
        ce::UseProgram use( display_ );
        // we render from this buffer, so set the vertex attribute object!
        positionBuffer.addAttribute(
            display_->attribute("vert"),
            4,
            GL_FLOAT,
            GL_FALSE,
            0,
            0u
        );
        //vbo and vao are bound in here
        positionBuffer.enableAttributes();
    }

    velocityBuffer.dataStore(
        ParticleCount * 4 * sizeof(float),
        NULL, 
        GL_DYNAMIC_COPY
    );
    float* velPtr = (float*) velocityBuffer.mapBufferRange(
        0, ParticleCount * 4 * sizeof(float),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
    );
    for ( std::size_t i = 0u; i < 4u*ParticleCount; i += 4u ) {
        velPtr[i]       = ce::Randf( -0.1f, 0.1f );
        velPtr[i+1u]    = ce::Randf( -0.1f, 0.1f );
        velPtr[i+2u]    = ce::Randf( -0.1f, 0.1f );
        velPtr[i+3u]    = 0.0f;
    }
    velocityBuffer.unmapBuffer();

    // why do we now store in the uniform buffer?
    ce::BufferObject attractorBuffer( GL_UNIFORM_BUFFER );
    attractorBuffer.dataStore(
        MaxAttractors*4*sizeof(float),
        NULL,
        GL_STATIC_DRAW
    );
    float attractorMass[MaxAttractors];
    for ( std::size_t i = 0; i < MaxAttractors; i++ ) {
        attractorMass[i] = 0.5f + ce::Randf(0.0f, 1.0f) * 0.5;
    }
    glBindBufferBase( GL_UNIFORM_BUFFER, 0, attractorBuffer.object() );

	positionTbo.setStore( GL_RGBA32F, positionBuffer );
	velocityTbo.setStore( GL_RGBA32F, velocityBuffer );
    /*TEMPORARY TEST STUFF ENDS HERE*/
    
    running_ = true;
    std::chrono::duration<float, std::ratio<1,1>> time( 0.0f );
    std::chrono::duration<float, std::ratio<1,1>> tdiff( 0.01f );
    while( running_ ) {
        auto tstart = std::chrono::steady_clock::now();
        updateContext_();

        if ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
            camera_.orbit( context_.mouse().dx, context_.mouse().dy );
        } else if ( SDL_GetMouseState( NULL, NULL ) & SDL_BUTTON(SDL_BUTTON_RIGHT) ) {
            camera_.translate( context_.mouse().dx, context_.mouse().dy );
        }

        camera_.update( tdiff.count() );

        /*HANDLE EVENTS HERE*/
        for (auto& event: context_.events_ ) {
            if ( event.type == SDL_MOUSEWHEEL ) {
                camera_.dolly( 0.5f*event.wheel.y );
            }
        }
        /*EVENT HANDLING ENDS HERE*/

        /*DISPLAY CODE BEGINS HERE*/
        // I never unbound the attractor buffer from the GL_UNIFORM_BUFFER target
        float* attractors = (float*) attractorBuffer.mapBufferRange(
            0, MaxAttractors * 4 * sizeof(float),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
        );
        for ( std::size_t i = 0u; i < 4*MaxAttractors; i += 4u ) {
            attractors[i]       = sinf(AttractorMovementSpeed*time.count() * (float)(i/4u + 4) * 7.5f * 20.0f) * 30.0f;
            attractors[i+1u]    = cosf(AttractorMovementSpeed*time.count() * (float)(i/4u + 7) * 3.9f * 20.0f) * 30.0f;
            attractors[i+2u]    = sinf(AttractorMovementSpeed*time.count() * (float)(i/4u + 3) * 5.3f * 20.0f) * cosf(AttractorMovementSpeed*time.count() * (float)(i + 5) * 9.1f) * 60.0f;
            attractors[i+3u]    = attractorMass[i / 4u];
        }
        attractorBuffer.unmapBuffer();

        //activate the compute program
        {
            ce::UseProgram use( compute_ );
            glBindImageTexture( 0, velocityTbo.object(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F );
            glBindImageTexture( 1, positionTbo.object(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F );
            glUniform1f( compute_->uniform("dt"), tdiff.count() );
            glDispatchCompute( ParticleGroupCount_, 1, 1 );
            glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
        }
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glDisable( GL_DEPTH_TEST );

        {
            ce::UseProgram use( display_ );
            display_->setUniform( "modelViewMatrix", camera_.matrix() );
            positionBuffer.bindVao();
            glEnable( GL_BLEND );
            glBlendFunc( GL_ONE, GL_ONE );
            glDrawArrays( GL_POINTS, 0, ParticleCount );
            positionBuffer.unbindVao();
        }

        auto tend = std::chrono::steady_clock::now();
        tdiff = tend - tstart;
        time += tdiff;
        /* DISPLAY CODE ENDS HERE*/

        window_.display();
    }

    glDeleteVertexArrays( 1, &renderVao );
}

void Application::initialize_() {
    if ( !ce::FileExists( "settings.txt" ) ) {
        std::cout << "No settings.txt file found!" << std::endl;
        std::exit( 0 );
    }
    
    std::ifstream fin( "settings.txt", std::ifstream::in );
    ce::DataParser parser( fin );
    parser.parse();

    ParticleGroupSize_ = unsigned( parser.node( "app" )->number( "particles_per_workgroup" ) );
    ParticleGroupCount_ = unsigned( parser.node( "app" )->number( "workgroups" ) );

    // a valid openGL context exists after this call
    window_.deserialize( parser.node( "app" )->node( "window" ) );
    camera_.deserialize( parser.node( "app" )->node( "camera" ) );
    camera_.setViewportAspectRatio( float(window_.width() ) / window_.height() );

    AttractorMovementSpeed = float( parser.node("app")->number("attractor_movement_speed") );

    std::vector<ce::Shader::ptr> shaders;
    shaders.emplace_back(
        ce::ShaderFromFile( "data/particles.comp.glsl", GL_COMPUTE_SHADER )
    );
    compute_ = ce::Program::ptr( new ce::Program( shaders ) );

    shaders.clear();
    shaders.emplace_back(
        ce::ShaderFromFile( "data/particles.vert.glsl", GL_VERTEX_SHADER )
    );
    shaders.emplace_back(
        ce::ShaderFromFile( "data/particles.frag.glsl", GL_FRAGMENT_SHADER )
    );
    display_ = ce::Program::ptr( new ce::Program( shaders ) );

}

void Application::updateContext_() {
    context_.events_.clear();

    // update real-time input
    int oldx = context_.mouse_.x;
    int oldy = context_.mouse_.y;
    int newx, newy;
    SDL_GetMouseState( &newx, &newy );
    context_.mouse_.dx = newx - oldx;
    context_.mouse_.dy = newy - oldy;
    context_.mouse_.x = newx;
    context_.mouse_.y = newy;

    // update events
    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        // close is the only event that the application shell handles
        if ( event.type == SDL_QUIT ) {
            running_ = false;
        } else {
            context_.events_.push_back( event );
        }
    }
}




