#include <opengl/Camera.h>
#include <serialization/ObjectNode.h>
#include <utils/Assert.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include <iostream>

using ce::Camera;
using ce::ObjectNode;

// hide local implementation
namespace {
    const float DegreesToRads = 3.141592653f / 180.0f;
    const float Pi = 3.141592653f;

    float Clamp( float val, float min, float max ) {
        if ( val < min ) {
            return min;
        }
        if ( val > max ) {
            return max;
        }
        return val;
    }

    // t should be limited to [0,1]
    float Lerp( float a, float b, float t ) {
        t = Clamp( t, 0.0f, 1.0f );
        return a + t*( b - a );
    }

    glm::vec3 Lerp( const glm::vec3& a, const glm::vec3& b, float t ) {
        t = Clamp( t, 0.0f, 1.0f );
        return a + t*( b - a );
    }
}

Camera::Camera()
    :   verticalFov_( 60.0f * DegreesToRads ),
        nearPlane_( 0.1f ),
        farPlane_( 1000.0f ),
        aspectRatio_( 1.3333333f ),
        position_( 0.0f, 0.0f, -10.0f ),
        targetPosition_( 0.0f, 0.0f, -10.0f ),
        focal_( 0.0f, 0.0f, 0.0f ),
        radius_( 40.0f ),
        targetRadius_( 60.0f ),
        minRadius_( 10.0f ),
        maxRadius_( 100.0f ),
        zoomFactor_( 0.07f * radius_ ),
        targetZoomFactor_( 0.07f * radius_ ),
        dollySensitivity_( 1.0f ),
        orbitSensitivity_( 1.0f ),
        panSensitivity_( 1.0f ),
        polar_( Pi / 2.0f ),
        azimuth_( 3.0f*Pi / 2.0f ),
        viewPerspective_( true )
        {}

float Camera::fov() const {
    return verticalFov_;
}

void Camera::setFov( float f ) {
    ASSERT( f > 0.0f, "error: field of view is negative" );
    verticalFov_ = f;
}

float Camera::nearPlane() const {
    return nearPlane_;
}

void Camera::setNearPlane( float near ) {
    ASSERT( near < farPlane_ && near > 0.0f, "error: near plane is negative or larger than the far plane" );
    nearPlane_ = near;
}

float Camera::farPlane() const {
    return farPlane_;
}

void Camera::setFarPlane( float far ) {
    ASSERT( far > nearPlane_, "error: far plane is less than the near plane" );
    farPlane_ = far;
}

float Camera::viewportAspectRatio() const {
    return aspectRatio_;
}

void Camera::setViewportAspectRatio( float aspect ) {
    ASSERT( aspect > 0.0f, "error: aspect ratio is negative" );
    aspectRatio_ = aspect;
}

void Camera::viewPerspective( bool p ) {
    viewPerspective_ = p;
}

void Camera::dolly( float movez ) {
    targetRadius_ += dollySensitivity_ * radius_ * movez;
    if ( targetRadius_ < minRadius_ ) {
        targetRadius_ = minRadius_;
    } else if ( targetRadius_ > maxRadius_ ) {
        targetRadius_ = maxRadius_;
    }
}

void Camera::orbit( float movex, float movey ) {
    polar_ -= orbitSensitivity_ * movey;
    polar_ = Clamp( polar_, 0.001f, Pi-0.001f );
    azimuth_ -= orbitSensitivity_ * movex;
    // limit the azimuth angle to the [0, 2PI] range.
    if ( azimuth_< 0.0f ) {
        azimuth_ += 2*Pi;
    }
    if ( azimuth_ > 2*Pi ) {
        azimuth_ -= 2*Pi;
    }
}

void Camera::translate( float movex, float movey ) {
    focal_ += - panSensitivity_ * radius_ * movex * right();
    focal_ += panSensitivity_ * radius_ * movey * up();
}

void Camera::update( float dt ) {
    /*
     * target targetZoomFactor_ defines how wide the orthographic proejection is
     * our projection width is scaled by our current radius, in order to simulate
     * "zooming" in and out in the ortho mode
     *
     * this value was arrived at by trial and error
     * using this value, the objects in view do not change size when switching to ortho
     */
    targetZoomFactor_ = 0.07f * targetRadius_;

    // update values
    zoomFactor_         = Lerp( zoomFactor_, targetZoomFactor_, Clamp( 20.0f*dt, 0.0f, 1.0f ) );
    radius_             = Lerp( radius_, targetRadius_, Clamp( 20.0f*dt, 0.0f, 1.0f ) );

    // calculate unit offset vector
    glm::vec3 offset = glm::vec3(
        sinf( polar_ ) * sinf( azimuth_ ),
        cosf( polar_ ),
        sinf( polar_ ) * cosf( azimuth_ )
    );
    // scale it by the radius
    targetPosition_ = radius_ * offset + focal_;
    position_ = Lerp( position_, targetPosition_, Clamp( 20.0f*dt, 0.0f, 1.0f ) );
}

glm::vec3 Camera::forward() const {
    glm::vec3 n = glm::normalize( position_ );
    return glm::vec3( -n[0], -n[1], -n[2] );
}

glm::vec3 Camera::right() const {
    float x = radius_ * sinf( polar_ ) * cosf( azimuth_ );
    float z = -radius_ * sinf( polar_ ) * sinf( azimuth_ );
    float n = sqrt( x*x + z*z );
    return glm::vec3( x/n, 0.0f, z/n );
}

glm::vec3 Camera::up() const {
    return glm::cross( right(), forward() );
}

glm::mat4 Camera::view() const {
    return glm::lookAt( position_, focal_, up() );
}

glm::mat4 Camera::projection() const {
    if ( viewPerspective_ ) {
        return glm::perspective( verticalFov_, aspectRatio_, nearPlane_, farPlane_ );
    } else {
        // return orthogonal projection
    }
}

glm::mat4 Camera::matrix() const {
    return projection() * view();
}

void Camera::deserialize( ObjectNode::node_ptr node ) {
    verticalFov_        = float( node->number("vertical_fov") );
    nearPlane_          = float( node->number("near_plane") );
    farPlane_           = float( node->number("far_plane") );
    minRadius_          = float( node->number("min_radius") );
    maxRadius_          = float( node->number("max_radius") );
    dollySensitivity_   = float( node->number("dolly_sensitivity") );
    orbitSensitivity_   = float( node->number("orbit_sensitivity") );
    panSensitivity_     = float( node->number("pan_sensitivity") );
    std::string pers = node->string( "view_as_perspective" );
    if ( pers == "true" ) {
        viewPerspective_ = true;
    } else if ( pers == "false" ) {
        viewPerspective_ = false;
    }
}

ObjectNode::node_ptr Camera::serialize() {
    auto node = std::shared_ptr<ObjectNode>( new ObjectNode( "camera" ) );
    node->addNumber( "vertical_fov", verticalFov_ );
    node->addNumber( "near_plane", nearPlane_ );
    node->addNumber( "far_plane", farPlane_ );
    node->addNumber( "min_radius", minRadius_ );
    node->addNumber( "max_radius", maxRadius_ );
    node->addNumber( "dolly_sensitivity", dollySensitivity_ );
    node->addNumber( "orbit_sensitivity", orbitSensitivity_ );
    node->addNumber( "pan_sensitivity", panSensitivity_ );
    if ( viewPerspective_ ) {
        node->addString( "view_as_perspective", "true" );
    } else {
        node->addString( "view_as_perspective", "false" );
    }
    return node;
}


