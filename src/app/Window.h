#ifndef WINDOW_H
#define WINDOW_H

#include <serialization/ISerializable.h>
#include <SDL2/SDL_video.h>

namespace ce {

/// \brief A platform-independent window.
/// This window contains a valid OpenGL 4.3 context.
class Window: public ISerializable {
    public:
        /// \brief Construct the window.
        /// The OpenGL context and window are ready for use after construction.
        Window();
        ~Window();

        Window( const Window& )             = delete;
        Window& operator=( const Window& )  = delete;
        Window( Window&& )                  = delete;
        Window& operator=( Window&& )       = delete;

        void deserialize( ObjectNode::node_ptr ) override;
        ObjectNode::node_ptr serialize() override;

        /// \brief Display the window and any changes made to the back buffer.
        void display();

        unsigned width() const;
        unsigned height() const;

    private:
        void initialize_();
        void initializeSDL_();
        void initializeOpenGL_();

        SDL_Window*     window_;
        SDL_GLContext   glContext_;
        int             width_;
        int             height_;
        int             glMajor_;
        int             glMinor_;
        int             stencilBits_;
        int             depthBits_;
        int             msBuffer_;  // multisample buffer
        int             msSamples_; // number of multisamples

};

}   //namespace ce

#endif // WINDOW_H

