#ifndef APPLICATION_H
#define APPLICATION_H

#include <app/Window.h>
#include <app/Context.h>
#include <opengl/Program.h>
#include <opengl/Camera.h>

namespace ce {

/// \brief The main application class.
/// All functionality of the application will be dependent on this class.
/// It is the responsibilty of this class to provide the following services:
/// -maintain the list of current events
/// -provide the application window
/// -provide real-time input
/// All of these services are available to application states via the Context-structure.
class Application {
    public:
        Application();
        ~Application() = default;

        void run();

    private:
        void initialize_();
        void updateContext_();

        bool            running_;
        Window          window_;
        Context         context_;
        Camera          camera_;
        Program::ptr    compute_;
        Program::ptr    display_;

        //TEMPORARY STUFF
        unsigned        ParticleGroupSize_;
        unsigned        ParticleGroupCount_;
};

}

#endif // APPLICATION_H
