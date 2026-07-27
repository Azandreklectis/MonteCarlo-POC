#include "CSVWriter.h"

CSVWriter::CSVWriter(const string& filename)
{
    file.open(filename);

    writeHeader();
}

CSVWriter::~CSVWriter()
{
    if (file.is_open())
        file.close();
}

void CSVWriter::writeHeader()
{
    file
        << "RunID,"
        << "LatticeSize,"
        << "Temperature,"
        << "CouplingConstant,"
        << "MagneticField,"
        << "MonteCarloSteps,"
        << "ThermalizationSteps,"
        << "MeasurementInterval,"
        << "RandomSeed,"

        << "InitialEnergy,"
        << "FinalEnergy,"
        << "AverageEnergy,"
        << "MinimumEnergy,"
        << "MaximumEnergy,"
        << "EnergyVariance,"
        << "EnergyStandardDeviation,"

        << "InitialMagnetization,"
        << "FinalMagnetization,"
        << "AverageMagnetization,"
        << "MinimumMagnetization,"
        << "MaximumMagnetization,"
        << "MagnetizationVariance,"
        << "MagnetizationStandardDeviation,"

        << "AcceptedMoves,"
        << "RejectedMoves,"
        << "TotalAttemptedMoves,"
        << "AcceptanceRatio,"

        << "UpSpins,"
        << "DownSpins,"

        << "SpecificHeat,"
        << "Susceptibility,"
        << "BinderCumulant,"

        << "ExecutionTimeMS,"
        << "SpinFile\n";
}

void CSVWriter::append(const DatasetRecord& r)
{
    file
        << r.runID << ','
        << r.latticeSize << ','
        << r.temperature << ','
        << r.couplingConstant << ','
        << r.magneticField << ','
        << r.monteCarloSteps << ','
        << r.thermalizationSteps << ','
        << r.measurementInterval << ','
        << r.randomSeed << ','

        << r.initialEnergy << ','
        << r.finalEnergy << ','
        << r.averageEnergy << ','
        << r.minimumEnergy << ','
        << r.maximumEnergy << ','
        << r.energyVariance << ','
        << r.energyStandardDeviation << ','

        << r.initialMagnetization << ','
        << r.finalMagnetization << ','
        << r.averageMagnetization << ','
        << r.minimumMagnetization << ','
        << r.maximumMagnetization << ','
        << r.magnetizationVariance << ','
        << r.magnetizationStandardDeviation << ','

        << r.acceptedMoves << ','
        << r.rejectedMoves << ','
        << r.totalAttemptedMoves << ','
        << r.acceptanceRatio << ','

        << r.upSpins << ','
        << r.downSpins << ','

        << r.specificHeat << ','
        << r.susceptibility << ','
        << r.binderCumulant << ','

        << r.executionTimeMS << ','
        << r.spinFile
        << '\n';
}