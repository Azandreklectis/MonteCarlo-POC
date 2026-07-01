#include <iostream>

#include "SimulationParameters/SimulationParameters.h"
#include "Simulation/IsingSimulation.h"

using namespace std;

int main()
{
    SimulationParameters params;

    IsingSimulation simulation(params);

    for (int i = 0; i < 100; i++)
    {
        simulation.initialize();   // New random lattice

        simulation.run();          // Monte Carlo simulation

        simulation.printResults(); // Save results to output.txt
    }

    return 0;
}