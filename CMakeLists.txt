cmake_minimum_required(VERSION 3.20)

project(OpenACC_Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenACC_Test
        main.cpp
        Entities/Particle.h
        SimulationParameters/SimulationParameters.h
        Simulation/IsingSimulation.h
        Dataset/DatasetWriter.h
        Utilities/RandomGenerator.h
)

target_compile_options(OpenACC_Test PRIVATE
        -acc
        -Minfo=accel
        -gpu=debug
)

target_link_options(OpenACC_Test PRIVATE
        -acc
)