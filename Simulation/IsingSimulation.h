#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>

#include "../Entities/Particle.h"
#include "../SimulationParameters/SimulationParameters.h"
#include "../Utilities/RandomGenerator.h"

using namespace std;

class IsingSimulation
{
private:

    vector<vector<Particle>> lattice;
    SimulationParameters params;
    vector<vector<RNGState>> rngStates;
    //random_device{}()
public:

    IsingSimulation(const SimulationParameters& params)
    : params(params)
    {
        // Allocate lattice
        lattice.resize(
            params.latticeSize,
            vector<Particle>(params.latticeSize)
        );

        // Allocate one RNG state per lattice site
        rngStates.resize(
            params.latticeSize,
            vector<RNGState>(params.latticeSize)
        );

        unsigned int seed = random_device{}();

        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                rngStates[i][j].state =
                    seed + i * params.latticeSize + j;
            }
        }

        initialize();
    }


    void initialize()
    {
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                double random =RandomGenerator::uniform(rngStates[i][j].state);

                lattice[i][j].spin =
                    (random < 0.5) ? -1 : 1;
            }
        }
    }

    bool acceptMove(double deltaEnergy, double random)
    {
        if (deltaEnergy <= 0)
        {
            return true;
        }

        double probability =
            exp(-deltaEnergy / params.temperature);

        return random <= probability;
    }

    double localEnergy(int row, int col)
    {
        int N = params.latticeSize;

        int spin = lattice[row][col].spin;

        int up    = lattice[(row - 1 + N) % N][col].spin;
        int down  = lattice[(row + 1) % N][col].spin;
        int left  = lattice[row][(col - 1 + N) % N].spin;
        int right = lattice[row][(col + 1) % N].spin;

        int neighbourSum = up + down + left + right;

        return -params.couplingConstant * spin * neighbourSum
               -params.magneticField * spin;
    }
    double totalEnergy()
    {
        double energy = 0;

        #pragma acc parallel loop collapse(2) reduction(+:energy)
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                energy += localEnergy(i, j);
            }
        }

        return energy / 2.0;
    }
    // serial waala part
    // void metropolisStep()
    // {
    //     uniform_int_distribution<> indexDist(0, params.latticeSize - 1);
    //     uniform_real_distribution<> probabilityDist(0.0, 1.0);
    //
    //     int row = indexDist(gen);
    //     int col = indexDist(gen);
    //
    //     double oldEnergy = localEnergy(row, col);
    //
    //     lattice[row][col].flip();
    //
    //     double newEnergy = localEnergy(row, col);
    //
    //     double deltaEnergy = newEnergy - oldEnergy;
    //
    //     if (deltaEnergy <= 0)
    //     {
    //         return;
    //     }
    //
    //     double probability =
    //         exp(-deltaEnergy / params.temperature);
    //
    //     if (probabilityDist(gen) > probability)
    //     {
    //         // Reject the flip
    //         lattice[row][col].flip();
    //     }
    // }

    // parallel waala part
    // void metropolisUpdate(int row, int col)
    // {
    //     double random = RandomGenerator::uniform(rngStates[row][col].state);
    //
    //     double oldEnergy = localEnergy(row, col);
    //
    //     lattice[row][col].flip();
    //
    //     double newEnergy = localEnergy(row, col);
    //
    //     double deltaEnergy = newEnergy - oldEnergy;
    //
    //     if (!acceptMove(deltaEnergy, random))
    //     {
    //         lattice[row][col].flip();
    //     }
    // }

    void metropolisUpdate(int row, int col)
    {
        int N = params.latticeSize;

        int spin = lattice[row][col].spin;

        int up =
            lattice[(row - 1 + N) % N][col].spin;

        int down =
            lattice[(row + 1) % N][col].spin;

        int left =
            lattice[row][(col - 1 + N) % N].spin;

        int right =
            lattice[row][(col + 1) % N].spin;

        int neighbourSum =
            up + down + left + right;

        double deltaEnergy =
            2.0 *
            spin *
            (
                params.couplingConstant *
                neighbourSum
                +
                params.magneticField
            );

        double random =
            RandomGenerator::uniform(
                rngStates[row][col].state
            );

        if (acceptMove(deltaEnergy, random))
        {
            lattice[row][col].flip();
        }
    }
    
    void updateBlack()
    {
        int N = params.latticeSize;

        for (int row = 0; row < N; row++)
        {
            int startCol = row % 2;

            for (int k = 0; k < N / 2; k++)
            {
                int col = startCol + 2 * k;

                metropolisUpdate(row, col);
            }
        }
    }
    void updateWhite()
    {
        int N = params.latticeSize;

        for (int row = 0; row < N; row++)
        {
            int startCol = (row + 1) % 2;

            for (int k = 0; k < N / 2; k++)
            {
                int col = startCol + 2 * k;

                metropolisUpdate(row, col);
            }
        }
    }

    // void run()
    // {
    //     int totalParticles =
    //         params.latticeSize * params.latticeSize;
    //
    //     for (int sweep = 0; sweep < params.monteCarloSteps; sweep++)
    //     {
    //         for (int i = 0; i < totalParticles; i++)
    //         {
    //             metropolisStep();
    //         }
    //     }
    // }

    void monteCarloStep()
    {
        updateBlack();

        updateWhite();
    }

    double magnetization()
    {
        int totalSpin = 0;

        #pragma acc parallel loop collapse(2) reduction(+:totalSpin)
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                totalSpin += lattice[i][j].spin;
            }
        }

        int totalParticles = params.latticeSize * params.latticeSize;

        return (double)totalSpin / totalParticles;
    }

    void printLattice()
    {
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                cout << lattice[i][j].spin << " ";
            }
            cout << endl;
        }
    }
    void printResults()
    {
        ofstream fout("output.txt", ios::app);

        fout << "========================================\n";
        fout << "Total Energy : " << totalEnergy() << endl;
        fout << "Magnetization : " << magnetization() << endl;
        fout << "Lattice:\n\n";

        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                fout << lattice[i][j].spin << " ";
            }

            fout << endl;
        }

        fout << "\n\n";

        fout.close();
    }
    const vector<vector<Particle>>& getLattice() const
    {
        return lattice;
    } // getter

};