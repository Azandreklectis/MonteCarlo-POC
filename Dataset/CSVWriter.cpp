#include "CSVWriter.h"

CSVWriter::CSVWriter(const string& filename)
{
    file.open(filename);

    writeHeader();
}

CSVWriter::~CSVWriter()
{
    if(file.is_open())
        file.close();
}

void CSVWriter::writeHeader()
{
    file
        << "RunID,"
        << "LatticeSize,"
        << "Temperature,"
        << "MagneticField,"
        << "CouplingConstant,"
        << "MonteCarloSteps,"
        << "AverageEnergy,"
        << "AverageMagnetization,"
        << "AcceptanceRatio,"
        << "ExecutionTimeMS,"
        << "RandomSeed,"
        << "SpinFile\n";
}

void CSVWriter::append(const DatasetRecord& r)
{
    file
        << r.runID << ','
        << r.latticeSize << ','
        << r.temperature << ','
        << r.magneticField << ','
        << r.couplingConstant << ','
        << r.monteCarloSteps << ','
        << r.averageEnergy << ','
        << r.averageMagnetization << ','
        << r.acceptanceRatio << ','
        << r.executionTimeMS << ','
        << r.randomSeed << ','
        << r.spinFile
        << '\n';
}