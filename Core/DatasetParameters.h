#pragma once

struct DatasetParameters
{
    //==================================================
    // Lattice
    //==================================================

    int latticeSize = 10;

    //==================================================
    // Temperature Sweep
    //==================================================

    double startTemperature = 0.1; // 0.0

    double endTemperature = 10; // 10.0

    double temperatureStep = 1;

    //==================================================
    // Magnetic Field Sweep
    //==================================================

    double startMagneticField = 1.0;

    double endMagneticField = 2.0;

    double magneticFieldStep = 1.0;


    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 500;

    int thermalizationSteps = 20;

    int measurementInterval = 10;

    //==================================================
    // Randomness
    //==================================================

    unsigned int initialSeed = 142;

    //==================================================
    // Output
    //==================================================

    bool saveSpinFiles = false;
};