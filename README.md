# OCR-Implementation
Scratch Implementation of OCR in C++.

# Project Structure
'''
├── CMakeLists.txt
├── config
│   ├── eval_config.json
│   └── train_config.json
├── data
│   ├── exampleData.csv
│   ├── exampleLabels.csv
│   └── weights.json
├── include
│   ├── GUI
│   │   └── interface.hpp
│   ├── json.hpp
│   ├── layer.hpp
│   ├── matrix.hpp
│   ├── neuralNetwork.hpp
│   ├── neuron.hpp
│   ├── OCR
│   │   └── OCR_test.hpp
│   └── utils
│       ├── math.hpp
│       └── read_csv.hpp
├── LICENSE
├── README.md
├── src
│   ├── GUI
│   │   ├── interface.cpp
│   │   ├── main.cpp
│   │   ├── raygui.h
│   │   └── sequential_implementation.cpp
│   ├── layer.cpp
│   ├── load_model.cpp
│   ├── main.cpp
│   ├── matrix.cpp
│   ├── neuralNetwork.cpp
│   ├── neuron.cpp
│   ├── save_model.cpp
│   └── utils
│       ├── math.cpp
│       └── read_csv.cpp
└── tests
    ├── backPropagation.cpp
    ├── download.png
    ├── feedForward.cpp
    ├── load_model.cpp
    ├── matrix.cpp
    ├── neuron.cpp
    ├── nn.cpp
    ├── OCR_test.cpp
    ├── opencv_test.cpp
    ├── pdf_test.cpp
    ├── raylib_test.cpp
    ├── Screenshots
    │   ├── Screenshot from 2024-12-28 22-40-04.png
    │   ├── Screenshot from 2024-12-28 23-57-44.png
    │   ├── Screenshot from 2024-12-28 23-58-23.png
    │   ├── Screenshot from 2024-12-28 23-58-44.png
    │   └── Screenshot from 2024-12-28 23-59-06.png
    ├── test-images
    │   ├── example_image.jpeg
    │   └── hand_written2.jpeg
    ├── train_model.cpp
    └── train_using_config.cpp
'''