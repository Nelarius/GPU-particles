#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <SDL2/SDL_events.h>
#include <vector>

namespace ce {

struct Mouse {
    Mouse()
    :   x(0),
        y(0),
        dx(0),
        dy(0)
        {}

    int x;
    int y;
    int dx;
    int dy;
};

/// \brief This holds variables that will be needed by the application components at run time.
struct Context {

    friend class Application;

    Context()
    :   mouse_()
        {}

    /// \brief Get real-time input for the mouse.
    Mouse                           mouse()     const { return mouse_; }
    /// \brief Get the current events.
    const std::vector<SDL_Event>&   getEvents() const { return events_; }

    private:
        Mouse                   mouse_;
        std::vector<SDL_Event>  events_;

};

}

#endif // CONTEXT_H_INCLUDED
