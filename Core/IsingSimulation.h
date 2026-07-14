#pragma once

#include <iostream>
#include <memory>
#include <random>

#include "SimulationParameters.h"
#include "RandomGenerator.h"

using namespace std;

class IsingSimulation
{
private:

    SimulationParameters params;

    int N;
    int totalSites;

    unique_ptr<int[]> spin;
    unique_ptr<RNGState[]> rngStates;

    unique_ptr<int[]> up;
    unique_ptr<int[]> down;
    unique_ptr<int[]> left;
    unique_ptr<int[]> right;

    unsigned int sweepState;

    inline int index(int row, int col) const
    {
        return row * N + col;
    }

public:

    IsingSimulation(const SimulationParameters& parameters)
        : params(parameters)
    {
        // Cache frequently used values
        N = params.latticeSize;
        totalSites = N * N;

        // Allocate memory
        spin = make_unique<int[]>(totalSites);

        rngStates = make_unique<RNGState[]>(totalSites);

        up = make_unique<int[]>(N);
        down = make_unique<int[]>(N);
        left = make_unique<int[]>(N);
        right = make_unique<int[]>(N);

        // Random seed
        unsigned int seed = random_device{}();

        sweepState = seed ^ 0x9E3779B9u;

        // Periodic boundary lookup tables
        for (int i = 0; i < N; i++)
        {
            up[i] = (i - 1 + N) % N;
            down[i] = (i + 1) % N;

            left[i] = (i - 1 + N) % N;
            right[i] = (i + 1) % N;
        }

        // Initialize RNG states
        for (int i = 0; i < totalSites; i++)
        {
            rngStates[i].state = RandomGenerator::splitmix32(seed + i);
        }

        // Initialize lattice
        initialize();
    }

    void initialize()
    {
        int positive = 0;
        int negative = 0;

        for (int i = 0; i < totalSites; i++)
        {
            double random = RandomGenerator::uniform(rngStates[i].state);

            spin[i] = (random < 0.5) ? -1 : 1;

            if (spin[i] == 1)
                positive++;
            else
                negative++;

            if (i < 20)
            {
                cout << i << " : " << random
                     << " -> " << spin[i] << endl;
            }
        }

        cout << "\nPositive = " << positive << endl;
        cout << "Negative = " << negative << endl;
    }

    void printLattice() const
    {
        for (int row = 0; row < N; row++)
        {
            for (int col = 0; col < N; col++)
            {
                cout << spin[index(row, col)] << " ";
            }

            cout << endl;
        }
    }
};