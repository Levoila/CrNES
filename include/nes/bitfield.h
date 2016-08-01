#ifndef BITFIELD_H
#define BITFIELD_H

#include <cstdint>

template<unsigned int N, typename T = uint8_t>
struct Bit
{
    static constexpr T mask = 1 << N;

    T data;

    Bit& operator=(bool val)
    {
        data = val ? (data | mask) : (data & ~mask);
        return *this;
    }

    operator bool() const
    {
        return (data & mask);
    }

    operator int() const
    {
        return (data & mask) > 0 ? 1 : 0;
    }
};

union Bitfield
{
    uint8_t raw;
    Bit<0> C;
    Bit<1> Z;
    Bit<2> I;
    Bit<3> D;
    Bit<4> B;
    Bit<6> V;
    Bit<7> N;
};

#endif
