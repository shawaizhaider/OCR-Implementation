#ifndef GUI_IMPLEMENTATION
#define GUI_IMPLEMENTATION

#include <iostream>
#include <string>
#include <raylib.h>
#include "../../src/GUI/raygui.h"
#include <vector>

static const int screenWidth = 1000;
static const int screenHeight = 800;
static const int MAX_FILEPATH_RECORDED = 4096;
static const int MAX_FILEPATH_SIZE = 2048;

void initializeWindow();
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
#endif // GUI_IMPLEMENTATION