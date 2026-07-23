#include <iostream>

#include "Core/SimulationParameters.h"
#include "Core/IsingSimulation.h"

#include <openacc.h>

using namespace std;

int main()
{
    cout << "NVIDIA devices: "
              << acc_get_num_devices(acc_device_nvidia) << '\n';

    cout << "Current device type: "
              << acc_get_device_type() << '\n';

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

    cout << "\nMemory layer initialized successfully.\n\n";

    cout << "Initial Lattice:\n";
    // simulation.printLattice();

    cout << "\nInitial Magnetization : "
         << simulation.calculateMagnetization()
         << endl;

    // Reset statistics before simulation
    simulation.resetStatistics();

    cout << "\nRunning Monte Carlo Simulation...\n";

    for (int step = 0; step < params.monteCarloSteps; step++)
    {
        simulation.monteCarloStep();
    }

    // Copy final lattice from GPU to CPU
    simulation.updateHost();

    cout << "\nSimulation Complete!\n";
    cout << "----------------------------------------\n";

    cout << "Acceptance Ratio : "
         << simulation.getAcceptanceRatio()
         << endl;

    cout << "Final Magnetization : "
         << simulation.calculateMagnetization()
         << endl;

    cout << "\nFinal Lattice:\n";
    // simulation.printLattice();

    return 0;
}