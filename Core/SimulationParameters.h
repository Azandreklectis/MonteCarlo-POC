#pragma once

struct DatasetParameters
{
    int latticeSize = 512;

    double startTemperature = 0.5;

    double endTemperature = 5.0;

    double temperatureStep = 0.2;

    double couplingConstant = 1.0;

    double magneticField = 0.0;

    int monteCarloSteps = 1000;

    unsigned int initialSeed = 0;

    bool saveSpinFiles = true;

    bool saveCSV = true;

    bool saveMetadata = true;
};