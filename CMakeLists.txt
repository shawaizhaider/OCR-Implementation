cmake_minimum_required(VERSION 3.3.2)
project(OCR_with_GUI)

set(CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)


if(NOT TARGET opencv_dnn)
    message(FATAL_ERROR "OpenCV DNN module not found. Please install OpenCV with DNN support.")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)
pkg_check_modules(Leptonica REQUIRED lept)


add_library(Tesseract::libtesseract INTERFACE IMPORTED)
set_target_properties(Tesseract::libtesseract PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Tesseract_INCLUDE_DIRS};${Leptonica_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${Tesseract_LIBRARIES};${Leptonica_LIBRARIES}"
)


include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/usr/local/include)
include_directories(include)


set(SOURCE_FILES
    src/GUI/main.cpp
    include/GUI/raygui.h
    src/GUI/tinyfiledialogs.cpp
    src/GUI/interface.cpp
    src/OCR_core.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
    src/GUI/handleLoadCustomModel.cpp
    src/GUI/handleTrainCustomModel.cpp
    src/utils/GUI_utils.cpp
    src/GUI/handleCharacterRecognition.cpp
    src/GUI/handleImageUpload.cpp
)

add_executable(OCR_with_GUI ${SOURCE_FILES})

target_link_libraries(OCR_with_GUI 
    ${OpenCV_LIBS}
    Tesseract::libtesseract 
    raylib 
    /usr/local/lib/libraylib.a 
    m pthread dl
)

#[==[
 Optional Executables

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
    src/load_model.cpp
)

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
    src/load_model.cpp
)

add_executable(
    neuralNetwork_test
    tests/nn.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
)

add_executable(
    feedforward_test
    tests/feedForward.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
)

add_executable(
    backpropagation_test
    tests/backPropagation.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
)


add_executable(
    load_model_test
    tests/load_model.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
)

add_executable(
    training_test
    tests/train_model.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
)

add_executable(
    training_test_using_config
    tests/train_using_config.cpp
    src/neuron.cpp
    src/layer.cpp 
    src/matrix.cpp 
    src/neuralNetwork.cpp
    src/save_model.cpp
    src/utils/math.cpp 
    src/utils/read_csv.cpp
    src/load_model.cpp
) 

add_executable(
    raylib_test
    raylib_test.cpp
)

add_executable(
    opencv_test
    tests/opencv_test.cpp
)

target_link_libraries(raylib_test raylib /usr/local/lib/libraylib.a m pthread dl)
target_link_libraries(opencv_test ${OpenCV_LIBS})

]==]