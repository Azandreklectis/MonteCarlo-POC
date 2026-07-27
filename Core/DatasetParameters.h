#pragma once

struct DatasetParameters
{
    //==================================================
    // Lattice
    //==================================================

    int latticeSize = 2048;

    //==================================================
    // Temperature Sweep
    //==================================================

    double startTemperature = 2.6;

    double endTemperature = 5.0;

    double temperatureStep = 0.2;

    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 2000;

    int thermalizationSteps = 300;

    int measurementInterval = 20;

    //==================================================
    // Randomness
    //==================================================

    unsigned int initialSeed = 67;

    //==================================================
    // Output
    //==================================================

    bool saveSpinFiles = true;
};