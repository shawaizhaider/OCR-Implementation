#include <iostream>
#include "../../include/GUI/interface.hpp"
#include "../../include/OCR/OCR_test.hpp"
#define RAYGUI_IMPLEMENTATION 
#include "raygui.h"

void drawFileListAndLoadButton(int &filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture, std::string &ocrText, bool &ocrMode);
void drawOCRResult(const std::string &ocrText);

int main() {
    initializeWindow();

    // State variables
    bool uploadMode = false;
    bool imageLoaded = false;
    bool ocrMode = false; // Define ocrMode here
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

        if (!uploadMode) {
            drawTitleAndButtons("Image to Text Converter", uploadMode);
        } else if (ocrMode) {
            drawOCRResult(ocrText);
        } else {
            if (filePathCounter == 0) {
                DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
                handleFileDrop(filePathCounter, filePaths, imageLoaded, loadedImage, texture);
            } else {
                drawFileListAndLoadButton(filePathCounter, filePaths, imageLoaded, loadedImage, texture, ocrText, ocrMode);
            }
            // drawLoadedImage(imageLoaded, texture);
        }
        EndDrawing();
    }

    cleanupResources(filePaths, filePathCounter, imageLoaded, loadedImage, texture);
    return 0;
}

void drawFileListAndLoadButton(int &filePathCounter, char *filePaths[], bool &imageLoaded, 
                               Image &loadedImage, Texture2D &texture, std::string &ocrText, 
                               bool &ocrMode) {
    const int buttonWidth = 300;
    const int buttonHeight = 70;
    const int buttonSpacing = 30;

    // Screen padding and layout constants
    const int imageMaxWidth = 600;
    const int imageMaxHeight = 400;
    const int screenMargin = 20;

    // Example: Draw text about dropped files
    DrawText("Dropped files:", screenMargin, screenMargin, 20, DARKGRAY);
    for (int i = 0; i < filePathCounter; i++) {
        DrawText(filePaths[i], screenMargin, screenMargin + 30 + 20 * i, 10, GRAY);
    }

    // Define the area to draw the image
    Rectangle imageRect = {
        (float)(GetScreenWidth() / 2 - imageMaxWidth / 2),
        (float)(screenMargin + 80),
        (float)imageMaxWidth,
        (float)imageMaxHeight
    };

    // Draw the loaded image (if any)
    if (imageLoaded) {
        float scaleX = imageRect.width / loadedImage.width;
        float scaleY = imageRect.height / loadedImage.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY; // Maintain aspect ratio

        Rectangle sourceRect = { 0, 0, (float)loadedImage.width, (float)loadedImage.height };
        Rectangle destRect = {
            imageRect.x,
            imageRect.y,
            loadedImage.width * scale,
            loadedImage.height * scale
        };

        // Center the image vertically within the rect if scaled
        destRect.x += (imageRect.width - destRect.width) / 2;
        destRect.y += (imageRect.height - destRect.height) / 2;

        DrawTexturePro(texture, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
    } else {
        // Placeholder rectangle for image area
        DrawRectangleRec(imageRect, LIGHTGRAY);
        DrawText("No Image Loaded", (int)(imageRect.x + 10), (int)(imageRect.y + 10), 20, DARKGRAY);
    }

    // Define the load button below the image area
    Rectangle loadButton = {
        (float)(GetScreenWidth() / 2 - buttonWidth / 2),
        (float)(imageRect.y + imageRect.height + buttonSpacing),
        (float)buttonWidth,
        (float)buttonHeight
    };

    // When pressed, load the image and run OCR
    if (GuiButton(loadButton, "Load Image")) {
        if (filePathCounter > 0) {
            loadedImage = LoadImage(filePaths[0]);
            texture = LoadTextureFromImage(loadedImage);
            imageLoaded = true;
            ocrText = runOCR(filePaths[0]);
            // Switch to OCR result mode
            ocrMode = true;
        }
    }
}


void drawOCRResult(const std::string &ocrText) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const int textBoxWidth = screenW - 200;
    const int textBoxHeight = screenH - 200;
    const int textBoxX = (screenW - textBoxWidth) / 2;
    const int textBoxY = (screenH - textBoxHeight) / 2;

    DrawText("OCR Result:", textBoxX, textBoxY - 40, 20, DARKGRAY);
    GuiTextBox((Rectangle){textBoxX, textBoxY, textBoxWidth, textBoxHeight}, (char *)ocrText.c_str(), ocrText.size(), false);

    Rectangle copyButton = {
        (float)(screenW/2 - 150),
        (float)(textBoxY + textBoxHeight + 20),
        300.f, 70.f
    };

    if (GuiButton(copyButton, "Copy to Clipboard")) {
        SetClipboardText(ocrText.c_str());
    }
}
