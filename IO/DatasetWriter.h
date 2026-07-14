#pragma once

#include <fstream>
#include <filesystem>

#include "../Core/SimulationParameters.h"

using namespace std;

class DatasetWriter
{
public:

    static void append(
        int runNumber,
        const string& imageName,
        const SimulationParameters& params,
        double energy,
        double magnetization)
    {
        filesystem::create_directories("../Dataset");

        string filename = "../Dataset/dataset.csv";

        bool fileExists = filesystem::exists(filename);

        ofstream file(filename, ios::app);

        if (!fileExists)
        {
            file << "Run,"
                 << "Image,"
                 << "Temperature,"
                 << "MagneticField,"
                 << "CouplingConstant,"
                 << "LatticeSize,"
                 << "MonteCarloSweeps,"
                 << "Energy,"
                 << "Magnetization\n";
        }

        file << runNumber << ","
             << imageName << ","
             << params.temperature << ","
             << params.magneticField << ","
             << params.couplingConstant << ","
             << params.latticeSize << ","
             << params.monteCarloSteps << ","
             << energy << ","
             << magnetization << "\n";

        file.close();
    }
};