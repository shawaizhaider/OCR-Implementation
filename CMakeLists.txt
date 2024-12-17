cmake_minimum_required(VERSION 3.3.2)
project(OCR-Implementation)

# Set the C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Add the executable
add_executable(
    OCR-Implementation
    src/main.cpp
    src/neuron.cpp
    src/layer.cpp
)