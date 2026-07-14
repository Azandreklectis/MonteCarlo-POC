#pragma once

struct SimulationParameters
{
    int latticeSize = 128;

    double temperature = 2.269;

    double couplingConstant = 1.0;

    double magneticField = 0.0;

    int monteCarloSteps = 1000;
};