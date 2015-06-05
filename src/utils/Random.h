#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <cstdint>

namespace ce {

    /// \brief Seed the generator with a random sequence drawn from the system's random device.
    /// The underlying generator is the new Park-Miller minimum standard linear congruential generator.
    void Randomize();

    /// \brief Get a random uint32 in [ a, b ].
    std::int32_t Randi( std::int32_t a, std::int32_t b );

    /// \brief Get a random float in [ a, b ).
    float Randf( float a, float b );

    /// \brief Get a random float in [ 0, 1 ).
    float Randf();

    /// \brief Get a random double in [ a, b ).
    double Randd( double a, double b );
}

#endif // RANDOM_H_INCLUDED
