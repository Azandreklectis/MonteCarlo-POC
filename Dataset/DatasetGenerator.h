#pragma once

#include <memory>
#include <string>

#include "../Core/SimulationParameters.h"
#include "../Core/DatasetParameters.h"

#include "CSVWriter.h"

class DatasetGenerator
{
public:
    DatasetGenerator();

    void generate();

private:
    void createOutput();

    void createCSVWriter();

    void writeMetadata();

    void runSingleSimulation(
        const SimulationParameters& params,
        int runID);

private:
    DatasetParameters datasetParams;

    std::string runFolder;

    std::unique_ptr<CSVWriter> csvWriter;
};