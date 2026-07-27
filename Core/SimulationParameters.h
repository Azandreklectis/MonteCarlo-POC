#pragma once

struct SimulationParameters
{
    //==================================================
    // Lattice
    //==================================================

    int latticeSize = 46300;

    //==================================================
    // Physical Parameters
    //==================================================

    double temperature = 2.26;

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 10000;

    int thermalizationSteps = 1000;

    int measurementInterval = 10;

    //==================================================
    // Random Number Generation
    //==================================================

    unsigned int randomSeed = 67;
};