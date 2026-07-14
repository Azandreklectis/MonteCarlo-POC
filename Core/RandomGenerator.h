#pragma once

struct RNGState
{
    unsigned int state;


};

class RandomGenerator
{
public:

    static unsigned int xorshift(unsigned int& state)
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;

        return state;
    }

    static double uniform(unsigned int& state)
    {
        return static_cast<double>(
            xorshift(state)
        ) / 4294967295.0;
    }

    static inline unsigned int splitmix32(unsigned int x)
    {
        x += 0x9e3779b9u;
        x = (x ^ (x >> 16)) * 0x85ebca6bu;
        x = (x ^ (x >> 13)) * 0xc2b2ae35u;
        x ^= (x >> 16);
        return x;
    }
};