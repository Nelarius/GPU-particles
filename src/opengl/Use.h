#ifndef USE_H_INCLUDED
#define USE_H_INCLUDED

namespace ce {

/// \brief A RAII for OpenGL resources which have to bound to the current context.
/// The type T is a pointer to the OpenGL resource.
/// The underlying type T has to have the use() and stopUsing() methods defined and implemented.
template<typename T>
class Use {
    public:
        Use( T& t )
        :   resource_( t )
            { resource_->use(); }

        ~Use() { resource_->stopUsing(); }

    private:
        T&  resource_;

};

#include <opengl/Program.h>
typedef Use<Program::ptr> UseProgram;

}


#endif // USE_H_INCLUDED
