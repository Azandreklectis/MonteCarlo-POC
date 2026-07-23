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

    void enterDeviceData()
    {
        int* spinPtr = spin.get();

        RNGState* rngPtr = rngStates.get();

        int* upPtr = up.get();
        int* downPtr = down.get();

        int* leftPtr = left.get();
        int* rightPtr = right.get();

        #pragma acc enter data copyin( \
                spinPtr[0:totalSites], \
                rngPtr[0:totalSites], \
                upPtr[0:N], \
                downPtr[0:N], \
                leftPtr[0:N], \
                rightPtr[0:N])
    }

    void updateHost()
    {
        int* spinPtr = spin.get();

    #pragma acc update self(spinPtr[0:totalSites])
    }

    void exitDeviceData()
    {
        int* spinPtr = spin.get();

        RNGState* rngPtr = rngStates.get();

        int* upPtr = up.get();
        int* downPtr = down.get();

        int* leftPtr = left.get();
        int* rightPtr = right.get();

        #pragma acc exit data delete( \
                spinPtr[0:totalSites], \
                rngPtr[0:totalSites], \
                upPtr[0:N], \
                downPtr[0:N], \
                leftPtr[0:N], \
                rightPtr[0:N])
    }

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

        enterDeviceData();

        initialize();

        updateHost();
    }

    ~IsingSimulation()
    {
        exitDeviceData();
    }

    void initialize()
    {
        int* spinPtr = spin.get();
        RNGState* rngPtr = rngStates.get();

        const int sites = totalSites;

        #pragma acc parallel loop present(spinPtr[0:sites], rngPtr[0:sites])
        for(int i = 0; i < sites; i++)
        {
            double random =
                RandomGenerator::uniform(rngPtr[i].state);

            spinPtr[i] =
                (random < 0.5) ? -1 : 1;
        }

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

    inline double deltaEnergy(int row, int col)
    {
        const int latticeSize = N;

        int* spinPtr = spin.get();
        int* upPtr = up.get();
        int* downPtr = down.get();
        int* leftPtr = left.get();
        int* rightPtr = right.get();

        const double J = params.couplingConstant;
        const double H = params.magneticField;

        int idx = row * latticeSize + col;

        int neighbourSum =
            spinPtr[upPtr[row] * latticeSize + col] +
            spinPtr[downPtr[row] * latticeSize + col] +
            spinPtr[row * latticeSize + leftPtr[col]] +
            spinPtr[row * latticeSize + rightPtr[col]];

        return
            2.0 *
            spinPtr[idx] *
            (
                J * neighbourSum +
                H
            );
    }

    inline bool acceptMove(double deltaEnergy, double random)
    {
        const double temperature = params.temperature;

        if (deltaEnergy <= 0.0)
        {
            return true;
        }

        double probability = exp(-deltaEnergy / temperature);

        return random <= probability;
    }

    inline void metropolisUpdate(int row, int col)
    {
        const int latticeSize = N;

        int* spinPtr = spin.get();

        RNGState* rngPtr = rngStates.get();

        int* upPtr = up.get();
        int* downPtr = down.get();
        int* leftPtr = left.get();
        int* rightPtr = right.get();

        const double J = params.couplingConstant;
        const double H = params.magneticField;

        int idx = row * latticeSize + col;

        double random =
            RandomGenerator::uniform(rngPtr[idx].state);

        int neighbourSum =
            spinPtr[upPtr[row] * latticeSize + col] +
            spinPtr[downPtr[row] * latticeSize + col] +
            spinPtr[row * latticeSize + leftPtr[col]] +
            spinPtr[row * latticeSize + rightPtr[col]];

        double dE =
            2.0 *
            spinPtr[idx] *
            (
                J * neighbourSum +
                H
            );

        if (acceptMove(dE, random))
        {
            spinPtr[idx] *= -1;
        }
    }

    void updateBlack()
    {
        for (int row = 0; row < N; row++)
        {
            int startCol = row % 2;

            for (int col = startCol; col < N; col += 2)
            {
                metropolisUpdate(row, col);
            }
        }
    }

    void updateWhite()
    {
        for (int row = 0; row < N; row++)
        {
            int startCol = (row + 1) % 2;

            for (int col = startCol; col < N; col += 2)
            {
                metropolisUpdate(row, col);
            }
        }
    }

    void monteCarloStep()
    {
        bool blackFirst =
            RandomGenerator::uniform(sweepState) < 0.5;

        if (blackFirst)
        {
            updateBlack();
            updateWhite();
        }
        else
        {
            updateWhite();
            updateBlack();
        }
    }


};