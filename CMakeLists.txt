cmake_minimum_required(VERSION 3.20)

project(OpenACC_Test)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(OpenACC_Test
        main.cpp
        Entities/Particle.h
        SimulationParameters/SimulationParameters.h
        Simulation/IsingSimulation.h
        Visualizer/Visualizer.h
        Dataset/DatasetWriter.h
        GIF/gif.h
        GIF/GIFGenerator.h
)
target_include_directories(OpenACC_Test PRIVATE
        indicators/include
)

target_link_libraries(OpenACC_Test PRIVATE
        ${OpenCV_LIBS}
        gomp
)

target_compile_options(OpenACC_Test PRIVATE -fopenacc)