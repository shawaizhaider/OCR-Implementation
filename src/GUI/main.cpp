#include <iostream>
#include "../../include/GUI/interface.hpp"
#define RAYGUI_IMPLEMENTATION 
#include "../../include/GUI/raygui.h"


int main() {
    initializeWindow();

    // State variables
    bool uploadMode = false;
    bool imageLoaded = false;
    bool trainMode = false;
    bool loadModelMode = false;
    bool charRecognitionMode = false;  
    bool ocrMode = false;
    char *filePaths[MAX_FILEPATH_RECORDED] = {0};
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }
    int filePathCounter = 0;
    Image loadedImage = {0};
    Texture2D texture = {0};
    std::string ocrText;

    while (!WindowShouldClose()) {
        BeginDrawing();
        drawDottedBackground({155, 168, 171, 255});

        if (!uploadMode && !trainMode && !loadModelMode && !charRecognitionMode) {
            drawTitleAndButtons("Image to Text Converter", uploadMode, trainMode, loadModelMode, charRecognitionMode);
        } else if (ocrMode) {
            drawOCRResult(ocrText);
            if (drawReturnToMainMenuButton()) {
                ocrMode = false;
            }
        } else if (trainMode) {
            handleTrainCustomModel();
            if(drawReturnToMainMenuButton()) {
                trainMode = false;
            }
        } else if (loadModelMode) {
            handleLoadCustomModel();
            if(drawReturnToMainMenuButton()) {
                loadModelMode = false;
            }
        } else if (charRecognitionMode) {
            handleCharacterRecognition();
            if(drawReturnToMainMenuButton()) {
                charRecognitionMode = false;
            }
        } else {
            // Replace the existing file handling code with handleImageUpload
            handleImageUpload(imageLoaded, ocrMode, loadedImage, texture, ocrText, filePaths, filePathCounter);
            
            if (drawReturnToMainMenuButton()) {
                uploadMode = false;
                if (imageLoaded) {
                    clearResources(imageLoaded, loadedImage, texture, filePathCounter);
                }
            }
        }
        EndDrawing();
    }

    cleanupResources(filePaths, filePathCounter, imageLoaded, loadedImage, texture);
    return 0;
}


