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
};