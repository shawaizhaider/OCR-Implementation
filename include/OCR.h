#ifndef OCR_H
#define OCR_H

#include "neuron.hpp"
#include "json.hpp"
#include "matrix.hpp"
#include "layer.hpp"
#include "neuralNetwork.hpp"
#include "utils/math.hpp"   
#include "utils/read_csv.hpp"
#include "OCR/OCR_test.hpp"
#include "GUI/interface.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <streambuf>
#include <time.h>
#include <raylib.h>
#include "../src/GUI/raygui.h"
#include <random>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using json = nlohmann::json;
using namespace std;

#endif   // OCR_H