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
        for (double magneticField = datasetParams.startMagneticField;
             magneticField <= datasetParams.endMagneticField;
             magneticField += datasetParams.magneticFieldStep)
        {
            SimulationParameters params;

            //==================================================
            // Lattice
            //==================================================

            params.latticeSize = datasetParams.latticeSize;

            //==================================================
            // Physical Parameters
            //==================================================

            params.temperature = temperature;
            params.magneticField = magneticField;
            params.couplingConstant = datasetParams.couplingConstant;

            //==================================================
            // Monte Carlo Parameters
            //==================================================

            params.monteCarloSteps = datasetParams.monteCarloSteps;
            params.thermalizationSteps = datasetParams.thermalizationSteps;
            params.measurementInterval = datasetParams.measurementInterval;

            //==================================================
            // Random Seed
            //==================================================

            params.randomSeed = datasetParams.initialSeed;

            //==================================================
            // Run Simulation
            //==================================================

            runSingleSimulation(params, runID++);
        }
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

    metadata << "Lattice Size          : " << datasetParams.latticeSize << '\n';
    metadata << "Temperature Start     : " << datasetParams.startTemperature << '\n';
    metadata << "Temperature End       : " << datasetParams.endTemperature << '\n';
    metadata << "Temperature Step      : " << datasetParams.temperatureStep << '\n';
    metadata << "Magnetic Field        : " << datasetParams.magneticField << '\n';
    metadata << "Coupling Constant     : " << datasetParams.couplingConstant << '\n';
    metadata << "Monte Carlo Steps     : " << datasetParams.monteCarloSteps << '\n';
    metadata << "Thermalization Steps  : " << datasetParams.thermalizationSteps << '\n';
    metadata << "Measurement Interval  : " << datasetParams.measurementInterval << '\n';
    metadata << "Initial Seed          : " << datasetParams.initialSeed << '\n';

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

cout << "\n========== Simulation Result ==========\n";

cout << "Initial Energy               : " << result.initialEnergy << '\n';
cout << "Final Energy                 : " << result.finalEnergy << '\n';
cout << "Average Energy               : " << result.averageEnergy << '\n';
cout << "Minimum Energy               : " << result.minimumEnergy << '\n';
cout << "Maximum Energy               : " << result.maximumEnergy << '\n';
cout << "Energy Variance              : " << result.energyVariance << '\n';
cout << "Energy Std Dev               : " << result.energyStandardDeviation << '\n';

cout << '\n';

cout << "Initial Magnetization        : " << result.initialMagnetization << '\n';
cout << "Final Magnetization          : " << result.finalMagnetization << '\n';
cout << "Average Magnetization        : " << result.averageMagnetization << '\n';
cout << "Minimum Magnetization        : " << result.minimumMagnetization << '\n';
cout << "Maximum Magnetization        : " << result.maximumMagnetization << '\n';
cout << "Magnetization Variance       : " << result.magnetizationVariance << '\n';
cout << "Magnetization Std Dev        : " << result.magnetizationStandardDeviation << '\n';

cout << '\n';

cout << "Accepted Moves              : " << result.acceptedMoves << '\n';
cout << "Rejected Moves              : " << result.rejectedMoves << '\n';
cout << "Total Attempted Moves       : " << result.totalAttemptedMoves << '\n';
cout << "Acceptance Ratio            : " << result.acceptanceRatio << '\n';

cout << '\n';

cout << "Up Spins                    : " << result.upSpins << '\n';
cout << "Down Spins                  : " << result.downSpins << '\n';

cout << '\n';

cout << "Specific Heat               : " << result.specificHeat << '\n';
cout << "Susceptibility              : " << result.susceptibility << '\n';
cout << "Binder Cumulant             : " << result.binderCumulant << '\n';

cout << '\n';

cout << "Execution Time (ms)         : " << result.executionTimeMS << '\n';

cout << "=======================================\n\n";
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

//==================================================
// Identification
//==================================================

record.runID = runID;

//==================================================
// Simulation Parameters
//==================================================

record.latticeSize = params.latticeSize;
record.temperature = params.temperature;
record.couplingConstant = params.couplingConstant;
record.magneticField = params.magneticField;

record.monteCarloSteps = params.monteCarloSteps;
record.thermalizationSteps = params.thermalizationSteps;
record.measurementInterval = params.measurementInterval;

record.randomSeed = params.randomSeed;

//==================================================
// Energy Statistics
//==================================================

record.initialEnergy = result.initialEnergy;
record.finalEnergy = result.finalEnergy;
record.averageEnergy = result.averageEnergy;
record.minimumEnergy = result.minimumEnergy;
record.maximumEnergy = result.maximumEnergy;
record.energyVariance = result.energyVariance;
record.energyStandardDeviation = result.energyStandardDeviation;

//==================================================
// Magnetization Statistics
//==================================================

record.initialMagnetization = result.initialMagnetization;
record.finalMagnetization = result.finalMagnetization;
record.averageMagnetization = result.averageMagnetization;
record.minimumMagnetization = result.minimumMagnetization;
record.maximumMagnetization = result.maximumMagnetization;
record.magnetizationVariance = result.magnetizationVariance;
record.magnetizationStandardDeviation = result.magnetizationStandardDeviation;

//==================================================
// Monte Carlo Statistics
//==================================================

record.acceptedMoves = result.acceptedMoves;
record.rejectedMoves = result.rejectedMoves;
record.totalAttemptedMoves = result.totalAttemptedMoves;
record.acceptanceRatio = result.acceptanceRatio;

//==================================================
// Spin Statistics
//==================================================

record.upSpins = result.upSpins;
record.downSpins = result.downSpins;

//==================================================
// Thermodynamic Quantities
//==================================================

record.specificHeat = result.specificHeat;
record.susceptibility = result.susceptibility;
record.binderCumulant = result.binderCumulant;

//==================================================
// Timing
//==================================================

record.executionTimeMS = result.executionTimeMS;

//==================================================
// Output
//==================================================

record.spinFile = spinFile;

csvWriter->append(record);

    cout << "Simulation " << runID << " Complete." << endl;
}