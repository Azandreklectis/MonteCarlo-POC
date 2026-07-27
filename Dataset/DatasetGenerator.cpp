#include "DatasetGenerator.h"

#include "CSVWriter.h"
#include "DatasetRecord.h"
#include "SpinWriter.h"

#include "../Core/SimulationParameters.h"
#include "../Core/DatasetParameters.h"
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

    int runID = 1;

    for (double temperature = datasetParams.startTemperature;
         temperature <= datasetParams.endTemperature;
         temperature += datasetParams.temperatureStep)
    {
        SimulationParameters params;

        params.latticeSize = datasetParams.latticeSize;
        params.temperature = temperature;
        params.magneticField = datasetParams.magneticField;
        params.couplingConstant = datasetParams.couplingConstant;
        params.monteCarloSteps = datasetParams.monteCarloSteps;
        params.randomSeed = datasetParams.initialSeed;

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

    metadata << "Lattice Size      : " << datasetParams.latticeSize << '\n';
    metadata << "Temperature Start : " << datasetParams.startTemperature << '\n';
    metadata << "Temperature End   : " << datasetParams.endTemperature << '\n';
    metadata << "Temperature Step  : " << datasetParams.temperatureStep << '\n';
    metadata << "Magnetic Field    : " << datasetParams.magneticField << '\n';
    metadata << "Coupling Constant : " << datasetParams.couplingConstant << '\n';
    metadata << "Monte Carlo Steps : " << datasetParams.monteCarloSteps << '\n';
    metadata << "Initial Seed      : " << datasetParams.initialSeed << '\n';

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

    string spinFile = "";

    if (datasetParams.saveSpinFiles)
    {
        spinFile = SpinWriter::save(
            runFolder,
            runID,
            simulation.getSpinData(),
            simulation.getTotalSites());

        cout << "Spin File : " << spinFile << endl;
    }

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

    record.randomSeed = params.randomSeed;
    record.spinFile = spinFile;

    csvWriter->append(record);

    cout << "Simulation " << runID << " Complete." << endl;
}