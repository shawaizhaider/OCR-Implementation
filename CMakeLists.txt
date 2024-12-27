cmake_minimum_required(VERSION 3.3.2)
project(OCR-Implementation)

# Set the C++ standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# # Enable debug mode and optional optimization flags
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")

add_executable(
    train-model
    tests/train_model.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/load_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

add_executable(
    predict
    tests/load_model.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/load_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

    #[===[
    OPTIONAL TESTS
add_executable(
    matrix_test
    tests/matrix.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

add_executable(
    neuron_test
    tests/neuron.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

add_executable(
    nn_test
    tests/nn.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

add_executable(
    feedForward_test
    tests/feedForward.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)

add_executable(
    backPropagation_test
    tests/backPropagation.cpp
    src/neuron.cpp  
    src/layer.cpp
    src/matrix.cpp
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp
    src/utils/read_csv.cpp
)
    ]===]
