cmake_minimum_required(VERSION 3.20)

project(OpenACC_Playground)

set(CMAKE_CXX_STANDARD 20)

add_executable(OpenACC_Playground
        main.cpp
)

target_compile_options(OpenACC_Playground PRIVATE
        -acc
        -Minfo=accel
        -gpu=ccall
)