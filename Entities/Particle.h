#pragma once

class Particle
{
public:
    int spin;

    Particle(int spin = 1)
        : spin(spin)
    {
    }

    void flip()
    {
        spin = -spin;
    }
};
