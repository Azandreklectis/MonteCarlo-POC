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

    vector<int> up;
    vector<int> down;
    vector<int> left;
    vector<int> right;

    unsigned int sweepState;

    vector<Particle> lattice;
    vector<RNGState> rngStates;
    SimulationParameters params;
    //random_device{}()

    inline int index(int row, int col) const
    {
        return row * params.latticeSize + col;
    }
public:

    void enterDeviceData()
    {

    }

    void exitDeviceData()
    {

    }

    void updateHostLattice()
    {

    }

    IsingSimulation(const SimulationParameters& params)
    : params(params)
    {

        up.resize(params.latticeSize);
        down.resize(params.latticeSize);
        left.resize(params.latticeSize);
        right.resize(params.latticeSize);

        int N = params.latticeSize;
        unsigned int seed = random_device{}();

        sweepState = seed ^ 0x9E3779B9u;

        for (int i = 0; i < N; i++)
        {
            up[i] = (i - 1 + N) % N;

            down[i] = (i + 1) % N;

            left[i] = (i - 1 + N) % N;

            right[i] = (i + 1) % N;
        }

        int totalParticles =
            params.latticeSize * params.latticeSize;

        lattice.resize(totalParticles);
        rngStates.resize(totalParticles);



        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                int idx = index(i, j);

                rngStates[idx].state = seed + idx;
            }
        }

        initialize();
    }


    void initialize()
    {
        //temp OFF
        //#pragma acc parallel loop collapse(2)
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                int idx = index(i, j);

                double random =
                    RandomGenerator::uniform(rngStates[idx].state);

                lattice[idx].spin =
                    (random < 0.5) ? -1 : 1;
            }
        }
    }

    double localEnergy(int row, int col)
    {
        int N = params.latticeSize;
        int idx = index(row, col);
        int spin = lattice[idx].spin;

        int upSpin = lattice[index(up[row], col)].spin;
        int downSpin = lattice[index(down[row], col)].spin;
        int leftSpin = lattice[index(row, left[col])].spin;
        int rightSpin =lattice[index(row, right[col])].spin;

        int neighbourSum = upSpin + downSpin + leftSpin + rightSpin;

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
        int idx = index(row, col);
        int spin = lattice[idx].spin;
        int upSpin = lattice[index(up[row], col)].spin;
        int downSpin = lattice[index(down[row], col)].spin;
        int leftSpin = lattice[index(row, left[col])].spin;
        int rightSpin =lattice[index(row, right[col])].spin;

        int neighbourSum = upSpin + downSpin + leftSpin + rightSpin;

        double deltaEnergy =
            2.0 *
            spin *
            (
                params.couplingConstant *
                neighbourSum
                +
                params.magneticField
            );

        double random = RandomGenerator::uniform(rngStates[idx].state);

        if (deltaEnergy <= 0)
        {
            lattice[idx].flip();
        }
        else
        {
            double probability =
                exp(-deltaEnergy / params.temperature);

            if (random <= probability)
            {
                lattice[idx].flip();
            }
        }
    }
    // call stack phatt gayi inline kar code
    void updateBlack()
    {
        int N = params.latticeSize;

        #pragma acc parallel loop collapse(2)
        for (int row = 0; row < N; row++)
        {
            for (int k = 0; k < N / 2; k++)
            {
                int startCol = row % 2;
                int col = startCol + 2 * k;

                metropolisUpdate(row, col);
            }
        }
    }
    void updateWhite()
    {
        int N = params.latticeSize;

        #pragma acc parallel loop collapse(2)
        for (int row = 0; row < N; row++)
        {
            for (int k = 0; k < N / 2; k++)
            {
                int startCol = (row + 1) % 2;
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

    // void monteCarloStep()
    // {
    //     updateBlack();
    //
    //     updateWhite();
    // }
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

    double magnetization()
    {
        int totalSpin = 0;

        #pragma acc parallel loop collapse(2) reduction(+:totalSpin)
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                int idx = index(i, j);

                totalSpin += lattice[idx].spin;
            }
        }

        int totalParticles =
            params.latticeSize * params.latticeSize;

        return static_cast<double>(totalSpin) / totalParticles;
    }

    void printLattice()
    {
        for (int i = 0; i < params.latticeSize; i++)
        {
            for (int j = 0; j < params.latticeSize; j++)
            {
                cout << lattice[index(i,j)].spin << " ";
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
                fout << lattice[index(i,j)].spin << " ";
            }

            fout << endl;
        }

        fout << "\n\n";

        fout.close();
    }
    const vector<Particle>& getLattice() const
    {
        return lattice;
    } // getter

};