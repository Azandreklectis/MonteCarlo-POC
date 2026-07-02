#include <iostream>

#include "SimulationParameters/SimulationParameters.h"
#include "Simulation/IsingSimulation.h"
#include "Visualizer/Visualizer.h"
#include "Dataset/DatasetWriter.h"
#include "GIF/GIFGenerator.h"

using namespace std;

int main()
{
    SimulationParameters params;

IsingSimulation simulation(params);

int frameInterval = 1;

for (int run = 1; run <= 3; run++)
{
    simulation.initialize();

    for (int step = 0; step < params.monteCarloSteps; step++)
    {
        simulation.monteCarloStep();

        if(step % frameInterval == 0)
        {
            Visualizer::saveFrame(
                simulation.getLattice(),
                params,
                run,
                step
            );
        }
    }

    double energy = simulation.totalEnergy();
    double magnetization = simulation.magnetization();

    simulation.printResults();

    Visualizer::saveImage(
        simulation.getLattice(),
        params,
        energy,
        magnetization,
        run
    );

    GIFGenerator::createGIF(
    run,
    params.latticeSize * 8,
    params.latticeSize * 8 + 40,
    frameInterval
);

    DatasetWriter::append(
        run,
        "run_" + to_string(run) + ".png",
        params,
        energy,
        magnetization
    );
}
    return 0;
}