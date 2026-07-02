#include <iostream>

#include "SimulationParameters/SimulationParameters.h"
#include "Simulation/IsingSimulation.h"
#include "Visualizer/Visualizer.h"
#include "Dataset/DatasetWriter.h"

using namespace std;

int main()
{
    SimulationParameters params;

    IsingSimulation simulation(params);

    for (int i = 0; i < 2; i++)
    {
        simulation.initialize();   // New random lattice

        simulation.run();          // Monte Carlo simulation

        // double energy = simulation.totalEnergy();
        // double magnetization = simulation.magnetization();

        simulation.printResults(); // Save results to output.txt

        Visualizer::saveImage(
            simulation.getLattice(), // create image
            params,
          simulation.totalEnergy(),
          simulation.magnetization(),
         i + 1);
    }

    return 0;
}