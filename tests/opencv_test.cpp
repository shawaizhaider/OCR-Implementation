#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("test.png");
    if (image.empty()) {
        std::cout << "Could not read the image!" << std::endl;
        return 1;
    }
    cv::imshow("Display window", image);
    cv::waitKey(0); // Wait for a keystroke
    return 0;
}
