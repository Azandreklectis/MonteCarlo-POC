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

    double startTemperature = 0.1; // 0.0

    double endTemperature = 10; // 10.0

    double temperatureStep = 0.1;

    //==================================================
    // Magnetic Field Sweep
    //==================================================

    double startMagneticField = 0.0;

    double endMagneticField = 10.0;

    double magneticFieldStep = 0.1;

    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 10000;

    int thermalizationSteps = 300;

    int measurementInterval = 20;

    //==================================================
    // Randomness
    //==================================================

    unsigned int initialSeed = 67;

    //==================================================
    // Output
    //==================================================

    bool saveSpinFiles = false;
};