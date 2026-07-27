#pragma once

struct SimulationResult
{
    //==================================================
    // Energy Statistics
    //==================================================

    double initialEnergy = 0.0;

    double finalEnergy = 0.0;

    double averageEnergy = 0.0;

    double minimumEnergy = 0.0;

    double maximumEnergy = 0.0;

    double energyVariance = 0.0;

    double energyStandardDeviation = 0.0;

    //==================================================
    // Magnetization Statistics
    //==================================================

    double initialMagnetization = 0.0;

    double finalMagnetization = 0.0;

    double averageMagnetization = 0.0;

    double minimumMagnetization = 0.0;

    double maximumMagnetization = 0.0;

    double magnetizationVariance = 0.0;

    double magnetizationStandardDeviation = 0.0;

    //==================================================
    // Monte Carlo Statistics
    //==================================================

    long long acceptedMoves = 0;

    long long rejectedMoves = 0;

    long long totalAttemptedMoves = 0;

    double acceptanceRatio = 0.0;

    //==================================================
    // Spin Statistics
    //==================================================

    int upSpins = 0;

    int downSpins = 0;

    //==================================================
    // Thermodynamic Quantities
    //==================================================

    double specificHeat = 0.0;

    double susceptibility = 0.0;

    double binderCumulant = 0.0;

    //==================================================
    // Timing
    //==================================================

    double executionTimeMS = 0.0;
};