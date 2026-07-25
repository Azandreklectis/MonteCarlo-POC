#include <iostream>
#include <filesystem>
#include <exception>

#include "Dataset/CSVWriter.h"

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

        CSVWriter writer("Output/dataset.csv");

        cout << "CSV Writer created successfully." << endl;

        //------------------------------------------------------
        // Create Dummy Record
        //------------------------------------------------------

        DatasetRecord record;

        record.runID = 1;
        record.latticeSize = 512;
        record.temperature = 2.20;
        record.magneticField = 0.0;
        record.couplingConstant = 1.0;
        record.monteCarloSteps = 1000;
        record.averageEnergy = -1.83;
        record.averageMagnetization = 0.91;
        record.acceptanceRatio = 0.24;
        record.executionTimeMS = 183.4;
        record.randomSeed = 12345;
        record.spinFile = "run000001.bin";

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