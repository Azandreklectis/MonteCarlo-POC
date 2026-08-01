#pragma once

#include <iostream>
#include <memory>
#include <random>
#include <chrono>

#include "SimulationParameters.h"
#include "RandomGenerator.h"
#include "SimulationResult.h"

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

    long long acceptedMoves = 0;

    //==================================================
    // Simulation Result
    //==================================================

    SimulationResult result;

    //==================================================
    // Energy Accumulators
    //==================================================

    double energySum = 0.0;
    double energySquaredSum = 0.0;

    double minimumEnergy = 0.0;
    double maximumEnergy = 0.0;

    //==================================================
    // Magnetization Accumulators
    //==================================================

    double magnetizationSum = 0.0;
    double magnetizationSquaredSum = 0.0;
    double magnetizationFourthPowerSum = 0.0;

    double minimumMagnetization = 0.0;
    double maximumMagnetization = 0.0;

    //==================================================
    // Measurement Statistics
    //==================================================

    int measurementCount = 0;

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

        // updateHost();
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

    #pragma acc routine seq
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
        const int latticeSize = N;
        const int sites = totalSites;

        int* spinPtr = spin.get();
        RNGState* rngPtr = rngStates.get();

        int* upPtr = up.get();
        int* downPtr = down.get();
        int* leftPtr = left.get();
        int* rightPtr = right.get();

        long long accepted = 0;

        const double J = params.couplingConstant;
        const double H = params.magneticField;
        const double temperature = params.temperature;

#pragma acc parallel loop \
reduction(+:accepted) \
present( \
spinPtr[0:sites], \
rngPtr[0:sites], \
upPtr[0:latticeSize], \
downPtr[0:latticeSize], \
leftPtr[0:latticeSize], \
rightPtr[0:latticeSize])
        for (int row = 0; row < latticeSize; row++)
        {
            int startCol = row % 2;

#pragma acc loop seq
            for (int col = startCol; col < latticeSize; col += 2)
            {
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

                if (dE <= 0.0 ||
                    random <= exp(-dE / temperature))
                {
                    spinPtr[idx] *= -1;
                    accepted++;
                }
            }
        }

        acceptedMoves += accepted;
    }

    void updateWhite()
    {
        const int latticeSize = N;
        const int sites = totalSites;

        int* spinPtr = spin.get();
        RNGState* rngPtr = rngStates.get();

        int* upPtr = up.get();
        int* downPtr = down.get();
        int* leftPtr = left.get();
        int* rightPtr = right.get();

        long long accepted = 0;

        const double J = params.couplingConstant;
        const double H = params.magneticField;
        const double temperature = params.temperature;

#pragma acc parallel loop \
reduction(+:accepted) \
present( \
spinPtr[0:sites], \
rngPtr[0:sites], \
upPtr[0:latticeSize], \
downPtr[0:latticeSize], \
leftPtr[0:latticeSize], \
rightPtr[0:latticeSize])
        for (int row = 0; row < latticeSize; row++)
        {
            int startCol = (row + 1) % 2;

#pragma acc loop seq
            for (int col = startCol; col < latticeSize; col += 2)
            {
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

                if (dE <= 0.0 ||
                    random <= exp(-dE / temperature))
                {
                    spinPtr[idx] *= -1;
                    accepted++;
                }
            }
        }

        acceptedMoves += accepted;
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


    double calculateEnergy()
    {
        const int latticeSize = N;
        const int sites = totalSites;

        int* spinPtr = spin.get();
        int* downPtr = down.get();
        int* rightPtr = right.get();

        double energy = 0.0;

#pragma acc parallel loop collapse(2) reduction(+:energy) \
present(spinPtr[0:sites],downPtr[0:latticeSize],rightPtr[0:latticeSize])
        for(int row=0; row<latticeSize; row++)
        {
            for(int col=0; col<latticeSize; col++)
            {
                int idx = row*latticeSize+col;

                int s = spinPtr[idx];

                energy +=
                    -params.couplingConstant*s*
                    (
                        spinPtr[downPtr[row]*latticeSize+col] +
                        spinPtr[row*latticeSize+rightPtr[col]]
                    );

                energy +=
                    -params.magneticField*s;
            }
        }

        return energy/totalSites;
    }

    double calculateMagnetization()
    {
        int* spinPtr = spin.get();

        long long totalSpin = 0;

#pragma acc parallel loop reduction(+:totalSpin) \
present(spinPtr[0:totalSites])
        for(int i=0;i<totalSites;i++)
        {
            totalSpin += spinPtr[i];
        }

        return static_cast<double>(totalSpin) / totalSites;
    }

    void resetStatistics()
    {
        acceptedMoves = 0;

        energySum = 0.0;
        energySquaredSum = 0.0;

        magnetizationSum = 0.0;
        magnetizationSquaredSum = 0.0;
        magnetizationFourthPowerSum = 0.0;

        minimumEnergy = 0.0;
        maximumEnergy = 0.0;

        minimumMagnetization = 0.0;
        maximumMagnetization = 0.0;

        measurementCount = 0;
    }

    double getAcceptanceRatio() const
    {
        long long attemptedMoves =
            static_cast<long long>(params.monteCarloSteps) *
            totalSites;

        if (attemptedMoves == 0)
            return 0.0;

        return static_cast<double>(acceptedMoves) /
               attemptedMoves;
    }

    SimulationResult runSimulation()
    {
        result = SimulationResult{};

        resetStatistics();

        auto start = chrono::high_resolution_clock::now();

        captureInitialState();

        thermalize();

        acceptedMoves = 0;

        performMeasurements();

        captureFinalState();

        auto end = chrono::high_resolution_clock::now();
        
        result.executionTimeMS =
            chrono::duration<double, milli>(end - start).count();

        finalizeStatistics();

        return result;
    }


    const int* getSpinData() const
    {
        return spin.get();
    }

    int getLatticeSize() const
    {
        return N;
    }

    int getTotalSites() const
    {
        return totalSites;
    }

    void thermalize()
    {
        for (int step = 0; step < params.thermalizationSteps; step++)
        {
            monteCarloStep();
        }
    }

    void measureSystem()
    {
        double energy = calculateEnergy();
        double magnetization = calculateMagnetization();

        energySum += energy;
        energySquaredSum += energy * energy;

        magnetizationSum += magnetization;
        magnetizationSquaredSum += magnetization * magnetization;
        magnetizationFourthPowerSum +=
            magnetization *
            magnetization *
            magnetization *
            magnetization;

        if (measurementCount == 0)
        {
            minimumEnergy = energy;
            maximumEnergy = energy;

            minimumMagnetization = magnetization;
            maximumMagnetization = magnetization;
        }
        else
        {
            if (energy < minimumEnergy)
                minimumEnergy = energy;

            if (energy > maximumEnergy)
                maximumEnergy = energy;

            if (magnetization < minimumMagnetization)
                minimumMagnetization = magnetization;

            if (magnetization > maximumMagnetization)
                maximumMagnetization = magnetization;
        }

        measurementCount++;
    }

    void performMeasurements()
    {
        for (int step = 0; step < params.monteCarloSteps; step++)
        {
            monteCarloStep();

            if ((step + 1) % params.measurementInterval == 0)
            {
                // updateHost();

                measureSystem();
            }
        }
    }

    void captureInitialState()
    {
        // updateHost();

        result.initialEnergy = calculateEnergy();
        result.initialMagnetization = calculateMagnetization();
    }

    void captureFinalState()
    {
        // updateHost();
        //
        result.finalEnergy = calculateEnergy();
        result.finalMagnetization = calculateMagnetization();
        //
        // int up = 0;
        // int down = 0;
        //
        // for (int i = 0; i < totalSites; i++)
        // {
        //     if (spin[i] == 1)
        //         up++;
        //     else
        //         down++;
        // }
        //
        // result.upSpins = up;
        // result.downSpins = down;
    }

    void finalizeStatistics()
    {
        if (measurementCount == 0)
            return;

        // Energy Statistics
        result.averageEnergy =
            energySum / measurementCount;

        result.minimumEnergy = minimumEnergy;
        result.maximumEnergy = maximumEnergy;

        result.energyVariance =
            (energySquaredSum / measurementCount) -
            (result.averageEnergy * result.averageEnergy);

        if (result.energyVariance < 0.0)
            result.energyVariance = 0.0;

        result.energyStandardDeviation =
            sqrt(result.energyVariance);


        // Magnetization Statistics
        result.averageMagnetization =
            magnetizationSum / measurementCount;

        result.minimumMagnetization =
            minimumMagnetization;

        result.maximumMagnetization =
            maximumMagnetization;

        result.magnetizationVariance =
            (magnetizationSquaredSum / measurementCount) -
            (result.averageMagnetization *
             result.averageMagnetization);

        if (result.magnetizationVariance < 0.0)
            result.magnetizationVariance = 0.0;

        result.magnetizationStandardDeviation =
            sqrt(result.magnetizationVariance);


        // Thermodynamic Quantities

        result.specificHeat =
(result.energyVariance * totalSites) /
(params.temperature *
 params.temperature);

        result.susceptibility =
(result.magnetizationVariance * totalSites) /
params.temperature;


        double meanM2 =
            magnetizationSquaredSum / measurementCount;

        double meanM4 =
            magnetizationFourthPowerSum / measurementCount;

        if (meanM2 > 0.0)
        {
            result.binderCumulant =
                1.0 -
                meanM4 /
                (3.0 * meanM2 * meanM2);
        }
        else
        {
            result.binderCumulant = 0.0;
        }

        result.acceptedMoves = acceptedMoves;

        result.totalAttemptedMoves =
            static_cast<long long>(params.monteCarloSteps) *
            totalSites;

        result.rejectedMoves =
            result.totalAttemptedMoves -
            result.acceptedMoves;

        result.acceptanceRatio =
            static_cast<double>(result.acceptedMoves) /
            result.totalAttemptedMoves;
    }
};