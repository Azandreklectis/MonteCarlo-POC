#include "DatasetGenerator.h"

#include <iostream>

DatasetGenerator::DatasetGenerator()
{
}

void DatasetGenerator::generate()
{
    std::cout << "======================================" << std::endl;
    std::cout << "Dataset Generator Started" << std::endl;
    std::cout << "======================================" << std::endl;

    createOutput();

    runSingleSimulation();
}

void DatasetGenerator::createOutput()
{
    std::cout << "Preparing output..." << std::endl;
}

void DatasetGenerator::runSingleSimulation()
{
    std::cout << "Running one simulation..." << std::endl;
}