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

    double startTemperature = 1; // 0.0

    double endTemperature = 3; // 10.0

    double temperatureStep = 1;

    //==================================================
    // Magnetic Field Sweep
    //==================================================

    double startMagneticField = 0.0;

    double endMagneticField = 1.0;

    double magneticFieldStep = 0.5;


    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 500;

    int thermalizationSteps = 500;

    int measurementInterval = 10;

    //==================================================
    // Randomness
    //==================================================

    unsigned int initialSeed = 53;

    //==================================================
    // Output
    //==================================================

    bool saveSpinFiles = false;
};