cmake_minimum_required(VERSION 3.20)

project(OpenACC_Test)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenACC_Test
        main.cpp
        Entities/Particle.h
        Core/SimulationParameters.h
        Core/IsingSimulation.h
        IO/DatasetWriter.h
        Core/RandomGenerator.h
        Core/Measurements.h
        IO/ConfigReader.h
        IO/Logger.h
        Benchmark/Timer.h
        Benchmark/Benchmark.h
)

target_compile_options(OpenACC_Test PRIVATE
        -acc
        -O3
        -Minfo=accel
)

target_link_options(OpenACC_Test PRIVATE
        -acc
)