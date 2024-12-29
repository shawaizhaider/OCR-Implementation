#ifndef GUI_IMPLEMENTATION
#define GUI_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "../../include/GUI/raygui.h"
#include "../../include/OCR/OCR_test.hpp"
#include <tesseract/baseapi.h>

static const int screenWidth = 1000;
static const int screenHeight = 800;
static const int MAX_FILEPATH_RECORDED = 4096;
static const int MAX_FILEPATH_SIZE = 2048;

void initializeWindow();
void drawFileListAndLoadButton(int &filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture, std::string &ocrText, bool &ocrMode);
void cleanupResources(char *filePaths[], int filePathCount, bool imageLoaded, Image &loadedImage, Texture2D &texture);
const char* getLoadedImagePath(char *filePaths[], int index);
void handleFileDrop(int &filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture);
void drawDottedBackground(Color color);
void drawTitleAndButtons(const char *titleText, bool &uploadMode);
void drawFileListAndLoadButton(int &filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture, std::string &ocrText, bool &ocrMode);
void drawLoadedImage(bool imageLoaded, Texture2D &texture);
void drawOCRResult(const std::string &ocrText);
void handleTrainCustomModel();
void handleLoadCustomModel();
void handleCharacterRecognition();
void drawTitleAndButtons(const char *titleText, bool &uploadMode, bool &trainMode, bool &loadModelMode, bool &charRecognitionMode);
bool drawReturnToMainMenuButton();
std::vector<std::string> WrapText(Font &font, const std::string &text, int fontSize, int maxWidth);
void drawOCRResult(const std::string &ocrText);

void handleImageUpload(bool &imageLoaded, bool &ocrMode, Image &loadedImage, 
                      Texture2D &texture, std::string &ocrText, 
                      char *filePaths[], int &filePathCounter);

void processImage(const char* imagePath, bool &imageLoaded, Image &loadedImage, 
                 Texture2D &texture, std::string &ocrText, char *filePaths[]);

void preprocessImage(const cv::Mat& input, cv::Mat& output);

void drawImageWithScaling(const Texture2D& texture, const Image& image, const Rectangle& area);

void processOCR(const char* imagePath, std::string& ocrText, bool& ocrMode);

void clearResources(bool& imageLoaded, Image& loadedImage, Texture2D& texture, int& filePathCounter);

#endif // GUI_IMPLEMENTATION