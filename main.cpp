#include <iostream>
#include <filesystem>
#include <exception>

#include "Dataset/CSVWriter.h"
#include "Core/SimulationParameters.h"
#include "Core/IsingSimulation.h"
#include "Utilities/FileManager.h"

using namespace std;
namespace fs = std::filesystem;

int main()
{
    try
    {
        cout << "======================================" << endl;
        cout << "Dataset Generator Debug Started" << endl;
        cout << "======================================" << endl;

        //------------------------------------------------------
        // Print current working directory
        //------------------------------------------------------

        cout << "Current Working Directory:" << endl;
        cout << fs::current_path() << endl << endl;

        //------------------------------------------------------
        // Check Output directory
        //------------------------------------------------------

        if (!fs::exists("Output"))
        {
            cout << "Output directory not found." << endl;
            cout << "Creating Output directory..." << endl;

            fs::create_directory("Output");

            if (fs::exists("Output"))
                cout << "Output directory created successfully." << endl;
            else
            {
                cerr << "Failed to create Output directory." << endl;
                return 1;
            }
        }
        else
        {
            cout << "Output directory exists." << endl;
        }

        cout << endl;

        //------------------------------------------------------
        // Create CSV Writer
        //------------------------------------------------------

        cout << "Creating CSV Writer..." << endl;

        std::string runFolder =
            FileManager::createRunDirectory();

        std::cout << "Run Folder: "
                  << runFolder
                  << std::endl;

        CSVWriter writer(runFolder + "/dataset.csv");
        
        cout << "CSV Writer created successfully." << endl;

        //------------------------------------------------------
        // Create Simulation Parameters
        //------------------------------------------------------

        SimulationParameters params;

        params.latticeSize = 512;
        params.temperature = 2.2;
        params.magneticField = 0.0;
        params.couplingConstant = 1.0;
        params.monteCarloSteps = 1000;

        cout << "Creating Ising Simulation..." << endl;

        IsingSimulation simulation(params);

        cout << "Running Simulation..." << endl;

        SimulationResult result =
            simulation.runSimulation();

        cout << "Simulation Finished." << endl;

        //------------------------------------------------------
        // Create Dataset Record
        //------------------------------------------------------

        DatasetRecord record;

        record.runID = 1;

        record.latticeSize = params.latticeSize;

        record.temperature = params.temperature;

        record.magneticField = params.magneticField;

        record.couplingConstant = params.couplingConstant;

        record.monteCarloSteps = params.monteCarloSteps;

        record.averageEnergy = result.averageEnergy;

        record.averageMagnetization =
            result.averageMagnetization;

        record.acceptanceRatio =
            result.acceptanceRatio;

        record.executionTimeMS =
            result.executionTimeMS;

        record.randomSeed = 0;

        record.spinFile = "";

        cout << "Appending record..." << endl;

        writer.append(record);

        cout << "Record appended successfully." << endl;

        cout << endl;
        cout << "Checking if CSV exists..." << endl;

        if (fs::exists("Output/dataset.csv"))
        {
            cout << "SUCCESS: dataset.csv created." << endl;
        }
        else
        {
            cerr << "ERROR: dataset.csv NOT found." << endl;
        }

        cout << endl;
        cout << "Program completed successfully." << endl;

        return 0;
    }
    catch (const exception& e)
    {
        cerr << endl;
        cerr << "========== EXCEPTION ==========" << endl;
        cerr << e.what() << endl;
        cerr << "===============================" << endl;

        return -1;
    }
    catch (...)
    {
        cerr << endl;
        cerr << "Unknown exception caught!" << endl;
        return -2;
    }
}