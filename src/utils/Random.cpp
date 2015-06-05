#include <utils/Random.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>

// hide the implementation details
namespace {

std::minstd_rand& global_urng() {
    static std::minstd_rand u;
    return u;
}

}

void ce::Randomize() {
    std::random_device rd{};
    global_urng().seed( rd() );
}

std::int32_t ce::Randi( std::int32_t a, std::int32_t b ) {
    static std::uniform_int_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}

float ce::Randf( float a, float b ) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}

float ce::Randf() {
    return ce::Randf( 0.0f, 1.0f );
}

double ce::Randd( double a, double b ) {
    static std::uniform_real_distribution<> d{};
    using parm_t = decltype(d)::param_type;
    return d( global_urng(), parm_t{ a, b } );
}
