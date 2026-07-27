#include "DatasetGenerator.h"

#include "CSVWriter.h"
#include "DatasetRecord.h"

#include "../Core/SimulationParameters.h"
#include "../Core/IsingSimulation.h"
#include "../Utilities/FileManager.h"

#include <iostream>
#include <fstream>

using namespace std;

DatasetGenerator::DatasetGenerator()
{
}

void DatasetGenerator::generate()
{
    cout << "======================================" << endl;
    cout << "Dataset Generator Started" << endl;
    cout << "======================================" << endl;

    createOutput();

    createCSVWriter();

    writeMetadata();

    SimulationParameters params;

    params.latticeSize = 512;
    params.temperature = 2.2;
    params.magneticField = 0.0;
    params.couplingConstant = 1.0;
    params.monteCarloSteps = 1000;

    runSingleSimulation(params, 1);int runID = 1;

    for (double temperature = 0.5; temperature <= 5.0; temperature += 0.2)
    {
        SimulationParameters params;

        params.latticeSize = 512;
        params.temperature = temperature;
        params.magneticField = 0.0;
        params.couplingConstant = 1.0;
        params.monteCarloSteps = 1000;

        runSingleSimulation(params, runID++);
    }


    cout << endl;
    cout << "Dataset Generation Complete." << endl;
}

void DatasetGenerator::createOutput()
{
    runFolder = FileManager::createRunDirectory();

    cout << "Run Folder : " << runFolder << endl;
}

void DatasetGenerator::createCSVWriter()
{
    csvWriter = make_unique<CSVWriter>(
        runFolder + "/dataset.csv");

    cout << "CSV Writer Created." << endl;
}

void DatasetGenerator::writeMetadata()
{
    ofstream metadata(runFolder + "/metadata.txt");

    if (!metadata.is_open())
    {
        cerr << "Failed to create metadata.txt" << endl;
        return;
    }

    metadata << "Monte Carlo Dataset Run\n";
    metadata << "=======================\n\n";

    metadata << "Lattice Size      : 512\n";
    metadata << "Temperature       : 2.2\n";
    metadata << "Magnetic Field    : 0.0\n";
    metadata << "Coupling Constant : 1.0\n";
    metadata << "Monte Carlo Steps : 1000\n";

    metadata.close();

    cout << "Metadata Created." << endl;
}

void DatasetGenerator::runSingleSimulation(
    const SimulationParameters& params,
    int runID)
{
    cout << endl;
    cout << "--------------------------------------" << endl;
    cout << "Simulation " << runID << endl;
    cout << "Temperature : " << params.temperature << endl;
    cout << "--------------------------------------" << endl;

    IsingSimulation simulation(params);

    SimulationResult result = simulation.runSimulation();

    DatasetRecord record;

    record.runID = runID;

    record.latticeSize = params.latticeSize;
    record.temperature = params.temperature;
    record.magneticField = params.magneticField;
    record.couplingConstant = params.couplingConstant;
    record.monteCarloSteps = params.monteCarloSteps;

    record.averageEnergy = result.averageEnergy;
    record.averageMagnetization = result.averageMagnetization;
    record.acceptanceRatio = result.acceptanceRatio;
    record.executionTimeMS = result.executionTimeMS;

    record.randomSeed = 0;
    record.spinFile = "";

    csvWriter->append(record);

    cout << "Simulation " << runID << " Complete." << endl;
}