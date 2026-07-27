#pragma once

#include <string>

using namespace std;

struct DatasetRecord
{
    //==================================================
    // Identification
    //==================================================

    int runID;

    //==================================================
    // Simulation Parameters
    //==================================================

    int latticeSize;

    double temperature;

    double couplingConstant;

    double magneticField;

    int monteCarloSteps;

    int thermalizationSteps;

    int measurementInterval;

    unsigned int randomSeed;

    //==================================================
    // Energy Statistics
    //==================================================

    double initialEnergy;

    double finalEnergy;

    double averageEnergy;

    double minimumEnergy;

    double maximumEnergy;

    double energyVariance;

    double energyStandardDeviation;

    //==================================================
    // Magnetization Statistics
    //==================================================

    double initialMagnetization;

    double finalMagnetization;

    double averageMagnetization;

    double minimumMagnetization;

    double maximumMagnetization;

    double magnetizationVariance;

    double magnetizationStandardDeviation;

    //==================================================
    // Monte Carlo Statistics
    //==================================================

    long long acceptedMoves;

    long long rejectedMoves;

    long long totalAttemptedMoves;

    double acceptanceRatio;

    //==================================================
    // Spin Statistics
    //==================================================

    int upSpins;

    int downSpins;

    //==================================================
    // Thermodynamic Quantities
    //==================================================

    double specificHeat;

    double susceptibility;

    double binderCumulant;

    //==================================================
    // Timing
    //==================================================

    double executionTimeMS;

    //==================================================
    // Output
    //==================================================

    string spinFile;
};