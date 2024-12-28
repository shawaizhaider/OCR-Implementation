cmake_minimum_required(VERSION 3.3.2)
project(OCR_with_GUI)

set(CMAKE_CXX_STANDARD 11)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Tesseract REQUIRED tesseract)
pkg_check_modules(Leptonica REQUIRED lept)

# Add Tesseract library
add_library(Tesseract::libtesseract INTERFACE IMPORTED)
set_target_properties(Tesseract::libtesseract PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Tesseract_INCLUDE_DIRS};${Leptonica_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${Tesseract_LIBRARIES};${Leptonica_LIBRARIES}"
)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(/usr/local/include)
include_directories(include)

# Source files
set(SOURCE_FILES
    src/GUI/main.cpp
    src/GUI/raygui.h
    src/GUI/interface.cpp
    tests/OCR_test.cpp
)

# Add executable
add_executable(OCR_with_GUI ${SOURCE_FILES})

# Link libraries
# Assuming you have raylib installed and available
target_link_libraries(OCR_with_GUI ${OpenCV_LIBS} Tesseract::libtesseract raylib /usr/local/lib/libraylib.a m pthread dl)
