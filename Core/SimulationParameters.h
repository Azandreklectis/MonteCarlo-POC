#pragma once

struct SimulationParameters
{
    int latticeSize = 100000;

    double temperature = 3.269;

    double couplingConstant = 1.0;

    double magneticField = 1.55;

    int monteCarloSteps = 100;
};