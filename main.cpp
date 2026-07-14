#include <iostream>

#include "Core/SimulationParameters.h"
#include "Core/IsingSimulation.h"

using namespace std;

int main()
{
    cout << "========================================\n";
    cout << "      OpenACC_Test - Ising Engine\n";
    cout << "========================================\n\n";

    SimulationParameters params;

    IsingSimulation simulation(params);

    cout << "Simulation created successfully!\n\n";

    cout << "Configuration:\n";
    cout << "----------------------------------------\n";
    cout << "Lattice Size      : " << params.latticeSize << " x " << params.latticeSize << '\n';
    cout << "Temperature       : " << params.temperature << '\n';
    cout << "Coupling Constant : " << params.couplingConstant << '\n';
    cout << "Magnetic Field    : " << params.magneticField << '\n';
    cout << "Monte Carlo Steps : " << params.monteCarloSteps << '\n';

    cout << "\nMemory layer initialized successfully.\n";

    simulation.printLattice();

    return 0;
}