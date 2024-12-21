cmake_minimum_required(VERSION 3.3.2)
project(OCR-Implementation)

# Set the C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Enable debug mode and optional optimization flags
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")

# Add the executable
add_executable(
    OCR-Implementation
    #src/main.cpp
    #tests/matrix.cpp
    #tests/neuron.cpp
    #tests/nn.cpp
    tests/feedForward.cpp
    src/neuron.cpp
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/utils/math.cpp
)