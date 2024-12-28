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
    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Optional: Preprocess image (thresholding)
    cv::Mat thresh;
    cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Initialize Tesseract OCR engine
    tesseract::TessBaseAPI tess;
    if (tess.Init(nullptr, "eng")) { // Set language to English
        std::cerr << "Could not initialize Tesseract!" << std::endl;
        return "";
    }

    // Set Tesseract to process the image
    tess.SetImage(thresh.data, thresh.cols, thresh.rows, thresh.channels(), thresh.step);
    std::string text = tess.GetUTF8Text();

    return text;
}

// int main() {
//     // If you want to test runOCR directly, pass a path:
//     // std::string text = runOCR("/path/to/some/image.jpg");
//     // std::cout << "Extracted Text:\n" << text << std::endl;
//     return 0;
// }
