#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <string>

std::string runOCR(const char* imagePath) {
    if (!imagePath) {
        std::cerr << "No image path provided!\n";
        return "";
    }
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Image not found!\n";
        return "";
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat thresh;
    cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    tesseract::TessBaseAPI tess;
    if (tess.Init(nullptr, "eng")) {
        std::cerr << "Could not initialize Tesseract!" << std::endl;
        return "";
    }
    tess.SetImage(thresh.data, thresh.cols, thresh.rows, thresh.channels(), thresh.step);
    std::string text = tess.GetUTF8Text();

    return text;
}

