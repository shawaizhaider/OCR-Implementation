#include "raylib.h"
#include "raygui.h"
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

// Helper: Draw title and buttons
void drawTitleAndButtons(const char *titleText, bool &uploadMode) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const int titleFontSize = 60;  
    const int titlePositionX = screenW/2 - MeasureText(titleText, titleFontSize)/2;
    const int titlePositionY = screenH/4;
    DrawText(titleText, titlePositionX, titlePositionY, titleFontSize, DARKGRAY);

    const int buttonWidth = 300;
    const int buttonHeight = 70;
    const int buttonSpacing = 30;  
    Rectangle uploadButton = {
        (float)(screenW/2 - buttonWidth/2),
        (float)(screenH/2),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle exitButton = {
        (float)(screenW/2 - buttonWidth/2),
        (float)(screenH/2 + buttonHeight + buttonSpacing),
        (float)buttonWidth, (float)buttonHeight
    };
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    if (GuiButton(uploadButton, "Upload Image")) {
        uploadMode = true;
    }
    if (GuiButton(exitButton, "Exit")) {
        CloseWindow();
    }
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
