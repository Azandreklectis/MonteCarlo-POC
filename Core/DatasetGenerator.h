#pragma once

struct DatasetParameters
{
    //==================================================
    // Lattice
    //==================================================

    int latticeSize = 512;

    //==================================================
    // Temperature Sweep
    //==================================================

    double startTemperature = 1.0;

    double endTemperature = 4.0;

    double temperatureStep = 0.1;

    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.0;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 1000;

    int thermalizationSteps = 200;

    int measurementInterval = 1;

    //==================================================
    // Randomness
    //==================================================

    unsigned int initialSeed = 42;

    //==================================================
    // Output
    //==================================================

    bool saveSpinFiles = true;
};