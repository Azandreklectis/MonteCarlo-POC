#pragma once

struct SimulationParameters
{
    int latticeSize = 46340;

    double temperature = 4.269;

    double couplingConstant = 1.0;

    double magneticField = 1.55;

    int monteCarloSteps = 1000;
};