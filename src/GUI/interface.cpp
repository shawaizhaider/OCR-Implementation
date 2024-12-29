#include "raylib.h"
#include "../../include/GUI/raygui.h"
#include <stdlib.h>
#include "../../include/GUI/interface.hpp"

// Helper: Initialize window and general state
void initializeWindow() {
    InitWindow(screenWidth, screenHeight, "Image to Text Converter");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);
    ClearBackground(WHITE);
}

// Helper: Cleanup resources
void cleanupResources(char *filePaths[], int filePathCount, bool imageLoaded, Image &loadedImage, Texture2D &texture) {
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
        RL_FREE(filePaths[i]);
    }
    if (imageLoaded) {
        UnloadImage(loadedImage);
        UnloadTexture(texture);
    }
    CloseWindow();
}

const char* getLoadedImagePath(char *filePaths[], int index) {
    return filePaths[index];
}

// Helper: Handle dropping files and loading images
void handleFileDrop(int &filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture) {
    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        for (int i = 0, offset = filePathCounter; i < (int)droppedFiles.count; i++) {
            if (filePathCounter < (MAX_FILEPATH_RECORDED - 1)) {
                TextCopy(filePaths[offset + i], droppedFiles.paths[i]);
                filePathCounter++;
            }
        }
        UnloadDroppedFiles(droppedFiles);
    }
    if (filePathCounter > 0) {
        // Example only loads first file
        loadedImage = LoadImage(filePaths[0]);
        texture = LoadTextureFromImage(loadedImage);
        imageLoaded = true;
    }
}

// Helper: Draw dotted background
void drawDottedBackground(Color color) {
    ClearBackground(color);
    for (int y = 0; y < GetScreenHeight(); y += 20) {
        for (int x = 0; x < GetScreenWidth(); x += 20) {
            DrawCircle(x, y, 2, Fade(DARKGRAY, 0.2f));
        }
    }
}

void drawTitleAndButtons(const char *titleText, bool &uploadMode, bool &trainMode, bool &loadModelMode, bool &charRecognitionMode) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const int titleFontSize = 60;  
    
    // Draw title
    const int titlePositionX = screenW / 2 - MeasureText(titleText, titleFontSize) / 2;
    const int titlePositionY = screenH / 8;
    DrawText(titleText, titlePositionX, titlePositionY, titleFontSize, DARKGRAY);

    // Button configurations
    const int buttonWidth = 300;
    const int buttonHeight = 60;
    const int buttonSpacing = 20;
    const float roundness = 0.2f;
    const int segments = 10;

    // Calculate button positions
    Rectangle uploadButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(screenH / 3),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle trainButton = {
        uploadButton.x,
        uploadButton.y + buttonHeight + buttonSpacing,
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle loadButton = {
        trainButton.x,
        trainButton.y + buttonHeight + buttonSpacing,
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle charRecogButton = {
        loadButton.x,
        loadButton.y + buttonHeight + buttonSpacing,
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle exitButton = {
        charRecogButton.x,
        charRecogButton.y + buttonHeight + buttonSpacing,
        (float)buttonWidth, (float)buttonHeight
    };

    // Set GUI style
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 2);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 2);

    // Draw button backgrounds
    Color buttonBg = Fade(LIGHTGRAY, 0.5f);
    DrawRectangleRounded(uploadButton, roundness, segments, buttonBg);
    DrawRectangleRounded(trainButton, roundness, segments, buttonBg);
    DrawRectangleRounded(loadButton, roundness, segments, buttonBg);
    DrawRectangleRounded(charRecogButton, roundness, segments, buttonBg);
    DrawRectangleRounded(exitButton, roundness, segments, buttonBg);

    // Handle button actions
    if (GuiButton(uploadButton, "Upload Image")) uploadMode = true;
    if (GuiButton(trainButton, "Train Custom Model")) trainMode = true;
    if (GuiButton(loadButton, "Load Custom Model")) loadModelMode = true;
    if (GuiButton(charRecogButton, "Character Recognition")) charRecognitionMode = true;
    if (GuiButton(exitButton, "Exit")) CloseWindow();
}


// Define a fixed area for the loaded image
const Rectangle IMAGE_AREA = { (float)(GetScreenWidth() / 2 - 300), 100.0f, 600.0f, 400.0f }; // Adjust width and height as needed

// Helper: Draw dropped file list and load button
void drawFileListAndLoadButton(int filePathCounter, char *filePaths[], bool &imageLoaded, Image &loadedImage, Texture2D &texture) {
    DrawText("Dropped files:", 100, 40, 20, DARKGRAY);
    for (int i = 0; i < filePathCounter; i++) {
        if (i % 2 == 0)
            DrawRectangle(0, 85 + 40*i, GetScreenWidth(), 40, Fade(LIGHTGRAY, 0.5f));
        else
            DrawRectangle(0, 85 + 40*i, GetScreenWidth(), 40, Fade(LIGHTGRAY, 0.3f));

        DrawText(filePaths[i], 120, 100 + 40*i, 10, GRAY);
    }
    const int buttonWidth = 300;
    const int buttonHeight = 70;
    const int buttonSpacing = 30; 
    Rectangle loadButton = {
        IMAGE_AREA.x,
        IMAGE_AREA.y + IMAGE_AREA.height + buttonSpacing,
        (float)buttonWidth,
        (float)buttonHeight
    };
    if (GuiButton(loadButton, "Load Image")) {
        if (filePathCounter > 0) {
            loadedImage = LoadImage(filePaths[0]);
            texture = LoadTextureFromImage(loadedImage);
            imageLoaded = true;
            // Now you can grab the path with getLoadedImagePath(filePaths, 0) and feed it to OCR
        }
    }
}

// Helper: Draw loaded image with scaling within IMAGE_AREA
void drawLoadedImage(bool imageLoaded, Texture2D &texture) {
    if (!imageLoaded) return;
    
    DrawTexturePro(texture,
                  {0, 0, (float)texture.width, (float)texture.height},
                  IMAGE_AREA,
                  {0, 0},
                  0.0f,
                  WHITE);
}


bool drawReturnToMainMenuButton() {
    const int buttonWidth = 200;
    const int buttonHeight = 50;
    const int buttonX = GetScreenWidth() - buttonWidth - 20; // Positioned at the bottom-right
    const int buttonY = GetScreenHeight() - buttonHeight - 20;

    Rectangle returnButton = {
        (float)buttonX, (float)buttonY,
        (float)buttonWidth, (float)buttonHeight
    };

    return GuiButton(returnButton, "Main Menu");
}


std::vector<std::string> WrapText(Font &font, const std::string &text, int fontSize, int maxWidth) {
    std::vector<std::string> lines;
    std::string currentLine;
    int currentWidth = 0;

    for (const char &ch : text) {
        int charWidth = MeasureTextEx(font, std::string(1, ch).c_str(), fontSize, 2).x;
        if (currentWidth + charWidth > maxWidth && ch == ' ') {
            lines.push_back(currentLine);
            currentLine.clear();
            currentWidth = 0;
        } else {
            currentLine += ch;
            currentWidth += charWidth;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}

void drawOCRResult(const std::string &ocrText) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int fontSize = 30;
    Font font = GetFontDefault();
    int textWidth = MeasureText("Character Recognition", fontSize);
    
    // Calculate box dimensions with padding
    float padding = 20.0f;
    float boxWidth = textWidth + padding * 2;
    float boxHeight = fontSize + padding * 2;
    
    // Center the box horizontally
    float boxX = (screenWidth - boxWidth) / 2;
    float boxY = 20;  // Fixed distance from top
    
    // Draw background box
    DrawRectangleRounded(
        {boxX, boxY, boxWidth, boxHeight},
        0.2f,  // roundness
        10,    // segments
        Fade(LIGHTGRAY, 0.5f)
    );
    
    // Draw text centered in box
    DrawText("Character Recognition",
            boxX + padding,
            boxY + padding/2,
            fontSize,
            BLACK);
            
    // Draw OCR result text below
    Vector2 textPos = {40, boxY + boxHeight + 40};
    DrawText(ocrText.c_str(), textPos.x, textPos.y, 20, BLACK);
}

void drawFileListAndLoadButton(int &filePathCounter, char *filePaths[], bool &imageLoaded, 
                               Image &loadedImage, Texture2D &texture, std::string &ocrText, 
                               bool &ocrMode) {
    const int buttonWidth = 300;
    const int buttonHeight = 70;
    const int buttonSpacing = 30;

    const int imageMaxWidth = 600;
    const int imageMaxHeight = 400;
    const int screenMargin = 20;

    DrawText("Dropped files:", screenMargin, screenMargin, 20, DARKGRAY);
    for (int i = 0; i < filePathCounter; i++) {
        DrawText(filePaths[i], screenMargin, screenMargin + 30 + 20 * i, 10, GRAY);
    }

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

        destRect.x += (imageRect.width - destRect.width) / 2;
        destRect.y += (imageRect.height - destRect.height) / 2;

        DrawTexturePro(texture, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
    } else {
      
        DrawRectangleRec(imageRect, LIGHTGRAY);
        DrawText("No Image Loaded", (int)(imageRect.x + 10), (int)(imageRect.y + 10), 20, DARKGRAY);
    }

    Rectangle loadButton = {
        (float)(GetScreenWidth() / 2 - buttonWidth / 2),
        (float)(imageRect.y + imageRect.height + buttonSpacing),
        (float)buttonWidth,
        (float)buttonHeight
    };

    if (GuiButton(loadButton, "Load Image")) {
        if (filePathCounter > 0) {
            loadedImage = LoadImage(filePaths[0]);
            texture = LoadTextureFromImage(loadedImage);
            imageLoaded = true;
            ocrText = runOCR(filePaths[0]);
            ocrMode = true;
        }
    }
}
