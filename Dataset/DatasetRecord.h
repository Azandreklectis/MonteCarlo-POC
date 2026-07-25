#pragma once

#include <string>

using namespace std;

struct DatasetRecord
{
    int runID;

    int latticeSize;

    double temperature;

    double magneticField;

    double couplingConstant;

    int monteCarloSteps;

    double averageEnergy;

    double averageMagnetization;

    double acceptanceRatio;

    double executionTimeMS;

    unsigned int randomSeed;

    string spinFile;
};