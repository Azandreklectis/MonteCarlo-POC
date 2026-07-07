#include <iostream>
#include <chrono>
#include <omp.h>

#include "SimulationParameters/SimulationParameters.h"
#include "Simulation/IsingSimulation.h"
//#include "Visualizer/Visualizer.h"
#include "Dataset/DatasetWriter.h"
//#include "GIF/GIFGenerator.h"
#include <openacc.h>


using namespace std;
using namespace std::chrono;

int main()
{
    SimulationParameters params;
    IsingSimulation simulation(params);

#include <openacc.h>

    cout << "Host devices      : "
         << acc_get_num_devices(acc_device_host)
         << endl;

    cout << "NVIDIA devices   : "
         << acc_get_num_devices(acc_device_nvidia)
         << endl;

    cout << "Radeon devices   : "
         << acc_get_num_devices(acc_device_radeon)
         << endl;

    cout << "Current device   : "
         << acc_get_device_type()
         << endl;
    //OwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwO
    int totalRuns = 1;
    int frameInterval = 1;
    //OwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwOwO issko change karna hai

    long long totalInitializationTime = 0;
    long long totalSimulationTime = 0;
    long long totalImageTime = 0;
    long long totalGIFTime = 0;
    long long totalDatasetTime = 0;

    auto totalStart = high_resolution_clock::now();

    //#pragma omp parallel for schedule(static)
    for (int run = 1; run <= totalRuns; run++)
    {
        cout << "\n=====================================================\n";
        cout << "Run " << run << " Started\n";
        cout << "=====================================================\n\n";

        auto runStart = high_resolution_clock::now();

        //---------------------------------------------------
        // Initialization
        //---------------------------------------------------

        cout << "Initialization Started...\n";

        auto initStart = high_resolution_clock::now();

        simulation.initialize();

        auto initEnd = high_resolution_clock::now();

        auto initTime =
            duration_cast<milliseconds>(initEnd - initStart);

        totalInitializationTime += initTime.count();

        cout << "Initialization Finished ("
             << initTime.count()
             << " ms)\n\n";

        //---------------------------------------------------
        // Monte Carlo Simulation
        //---------------------------------------------------

        cout << "Monte Carlo Simulation Started...\n";

        auto simulationStart = high_resolution_clock::now();

        for (int step = 0; step < params.monteCarloSteps; step++)
        {
            simulation.monteCarloStep();

            cout << "\rSweep "
                << step + 1
                << " / "
                << params.monteCarloSteps;

            cout.flush();

            // if (step % frameInterval == 0)
            // {
            //     Visualizer::saveFrame(
            //         simulation.getLattice(),
            //         params,
            //         run,
            //         step
            //     );
            // }
        }

        auto simulationEnd = high_resolution_clock::now();

        auto simulationTime =
            duration_cast<milliseconds>(
                simulationEnd - simulationStart
            );

        //#pragma omp atomic
        totalSimulationTime += simulationTime.count();

        cout << "Monte Carlo Simulation Finished ("
             << simulationTime.count()
             << " ms)\n\n";

        //---------------------------------------------------
        // Compute Statistics
        //---------------------------------------------------

        double energy = simulation.totalEnergy();
        double magnetization = simulation.magnetization();

        simulation.printResults();

        cout << "\nSimulation Statistics\n";
        cout << "----------------------------------------\n";
        cout << "Temperature        : " << params.temperature << endl;
        cout << "Energy             : " << energy << endl;
        cout << "Magnetization      : " << magnetization << endl;
        cout << "Monte Carlo Steps  : " << params.monteCarloSteps << endl;
        cout << endl;

        //---------------------------------------------------
        // Final Image
        //---------------------------------------------------
        //
        // cout << "Final Image Generation Started...\n";
        //
        // auto imageStart = high_resolution_clock::now();
        // //
        // // Visualizer::saveImage(
        // //     simulation.getLattice(),
        // //     params,
        // //     energy,
        // //     magnetization,
        // //     run
        // // );
        //
        // auto imageEnd = high_resolution_clock::now();
        //
        // auto imageTime =
        //     duration_cast<milliseconds>(
        //         imageEnd - imageStart
        //     );
        //
        // totalImageTime += imageTime.count();
        //
        // cout << "Final Image Generation Finished ("
        //      << imageTime.count()
        //      << " ms)\n\n";

        //---------------------------------------------------
        // GIF Generation
        //---------------------------------------------------
        //
        // cout << "GIF Generation Started...\n";
        //
        // auto gifStart = high_resolution_clock::now();
        //
        // GIFGenerator::createGIF(
        //     run,
        //     params.latticeSize * 8,
        //     params.latticeSize * 8 + 40,
        //     frameInterval
        // );
        //
        // auto gifEnd = high_resolution_clock::now();
        //
        // auto gifTime =
        //     duration_cast<milliseconds>(
        //         gifEnd - gifStart
        //     );
        //
        // totalGIFTime += gifTime.count();
        //
        // cout << "GIF Generation Finished ("
        //      << gifTime.count()
        //      << " ms)\n\n";

        //---------------------------------------------------
        // Dataset Writing
        //---------------------------------------------------

        cout << "Dataset Writing Started...\n";

        auto datasetStart = high_resolution_clock::now();
        // #pragma omp critical
        // {
        //     DatasetWriter::append(
        //         run,
        //         "run_" + to_string(run) + ".png",
        //         params,
        //         energy,
        //         magnetization
        //     );
        // }
        DatasetWriter::append(
               run,
               "run_" + to_string(run) + ".png",
               params,
               energy,
               magnetization
           );

        auto datasetEnd = high_resolution_clock::now();

        auto datasetTime =
            duration_cast<milliseconds>(
                datasetEnd - datasetStart
            );

        totalDatasetTime += datasetTime.count();

        cout << "Dataset Writing Finished ("
             << datasetTime.count()
             << " ms)\n\n";

        //---------------------------------------------------
        // Run Summary
        //---------------------------------------------------

        auto runEnd = high_resolution_clock::now();

        auto runTime =
            duration_cast<milliseconds>(
                runEnd - runStart
            );

        cout << "=====================================================\n";
        cout << "Run " << run << " Completed\n";
        cout << "=====================================================\n";

        cout << "Initialization : " << initTime.count() << " ms\n";
        cout << "Simulation     : " << simulationTime.count() << " ms\n";
        //cout << "Image          : " << imageTime.count() << " ms\n";
       // cout << "GIF            : " << gifTime.count() << " ms\n";
        cout << "Dataset        : " << datasetTime.count() << " ms\n";
        cout << "----------------------------------------\n";
        cout << "Total Run Time : " << runTime.count() << " ms\n\n";
    }

    auto totalEnd = high_resolution_clock::now();

    auto totalTime =
        duration_cast<milliseconds>(
            totalEnd - totalStart
        );

    cout << "\n=====================================================\n";
    cout << "PROJECT SUMMARY\n";
    cout << "=====================================================\n";

    cout << "Total Runs             : " << totalRuns << endl;
    cout << "Total Time             : " << totalTime.count() << " ms\n";
    cout << "Average Initialization : " << totalInitializationTime / totalRuns << " ms\n";
    cout << "Average Simulation     : " << totalSimulationTime / totalRuns << " ms\n";
    cout << "Average Image          : " << totalImageTime / totalRuns << " ms\n";
    cout << "Average GIF            : " << totalGIFTime / totalRuns << " ms\n";
    cout << "Average Dataset        : " << totalDatasetTime / totalRuns << " ms\n";

    cout << "=====================================================\n";

    return 0;
}