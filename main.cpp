#include "Dataset/CSVWriter.h"

int main()
{
    CSVWriter writer("Output/dataset.csv");

    DatasetRecord record;

    record.runID = 1;
    record.latticeSize = 512;
    record.temperature = 2.20;
    record.magneticField = 1.55;
    record.couplingConstant = 1.0;
    record.monteCarloSteps = 1000;

    record.averageEnergy = -1.83;
    record.averageMagnetization = 0.91;
    record.acceptanceRatio = 0.24;

    record.executionTimeMS = 182.4;

    record.randomSeed = 12345;

    record.spinFile = "run000001.bin";

    writer.append(record);

    return 0;
}