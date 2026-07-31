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

    double startTemperature = 2.3; // 0.0

    double endTemperature = 2.3; // 10.0

    double temperatureStep = 1;

    //==================================================
    // Magnetic Field Sweep
    //==================================================

    double startMagneticField = 0.0;

    double endMagneticField = 0.0;

    double magneticFieldStep = 1.0;


    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 5000;

    int thermalizationSteps = 2000;

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