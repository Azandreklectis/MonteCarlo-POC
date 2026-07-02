#pragma once

class SimulationParameters
{
public:
    int latticeSize; // n*n
    double temperature; // t
    double magneticField; // h
    double couplingConstant; // j

    int monteCarloSteps;

    SimulationParameters(
        int latticeSize = 50,
        double temperature = 2.5,
        double magneticField = 0.0,
        double couplingConstant = 1.0,
        int monteCarloSteps = 10000
    )
        : latticeSize(latticeSize),
          temperature(temperature),
          magneticField(magneticField),
          couplingConstant(couplingConstant),
          monteCarloSteps(monteCarloSteps)
    {
    }
};