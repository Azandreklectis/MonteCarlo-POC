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

    double startTemperature = 2.269; // 0.0

    double endTemperature = 2.269; // 10.0

    double temperatureStep = 0.5;

    //==================================================
    // Magnetic Field Sweep
    //==================================================

    double startMagneticField = 0.0;

    double endMagneticField = 0.0;

    double magneticFieldStep = 0.2;


    //==================================================
    // Physical Parameters
    //==================================================

    double couplingConstant = 1.0;

    double magneticField = 0.55;

    //==================================================
    // Monte Carlo Parameters
    //==================================================

    int monteCarloSteps = 5000;

    int thermalizationSteps = 200000;

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