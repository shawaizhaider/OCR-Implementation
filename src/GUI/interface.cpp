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

void drawTitleAndButtons(const char *titleText, bool &uploadMode, bool &trainMode, bool &loadModelMode, bool &charRecognitionMode) {
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const int titleFontSize = 60;  
    const int titlePositionX = screenW / 2 - MeasureText(titleText, titleFontSize) / 2;
    const int titlePositionY = screenH / 8;
    DrawText(titleText, titlePositionX, titlePositionY, titleFontSize, DARKGRAY);

    const int buttonWidth = 300;
    const int buttonHeight = 60; // Adjusted to fit all buttons
    const int buttonSpacing = 20;

    // Button positions
    Rectangle uploadButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(screenH / 3),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle trainButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(uploadButton.y + buttonHeight + buttonSpacing),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle loadButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(trainButton.y + buttonHeight + buttonSpacing),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle charRecogButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(loadButton.y + buttonHeight + buttonSpacing),
        (float)buttonWidth, (float)buttonHeight
    };
    Rectangle exitButton = {
        (float)(screenW / 2 - buttonWidth / 2),
        (float)(charRecogButton.y + buttonHeight + buttonSpacing),
        (float)buttonWidth, (float)buttonHeight
    };

    // Set GUI styles
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    // Buttons and corresponding actions
    if (GuiButton(uploadButton, "Upload Image")) {
        uploadMode = true;
    }
    if (GuiButton(trainButton, "Train Custom Model")) {
        trainMode = true; // Switch to train mode
    }
    if (GuiButton(loadButton, "Load Custom Model")) {
        loadModelMode = true; // Switch to load model mode
    }
    if (GuiButton(charRecogButton, "Character Recognition")) {
        charRecognitionMode = true; // Switch to character recognition mode
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
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();
    const int textBoxWidth = screenW - 300;
    const int textBoxHeight = screenH - 300;
    const int textBoxX = (screenW - textBoxWidth) / 2;
    const int textBoxY = (screenH - textBoxHeight) / 2;

    Font defaultFont = GetFontDefault(); // Using Raylib's default font
    const int titleFontSize = 30;
    const int contentFontSize = 20;
    const Color titleColor = DARKGRAY;

    // Draw the title
    DrawTextEx(defaultFont, "OCR Result:", (Vector2){textBoxX, textBoxY - 50}, titleFontSize, 2, titleColor);

    // Scrollable area for large OCR text
    static int scrollOffset = 0; // Keep track of scroll position
    const int scrollSpeed = 20;

    Rectangle scrollArea = {textBoxX, textBoxY, textBoxWidth, textBoxHeight};
    BeginScissorMode(scrollArea.x, scrollArea.y, scrollArea.width, scrollArea.height);
    {
        int textY = textBoxY - scrollOffset;

        // Split the OCR text into lines that fit the text box width
        std::vector<std::string> wrappedLines = WrapText(defaultFont, ocrText, contentFontSize, textBoxWidth - 20);
        for (const auto &line : wrappedLines) {
            DrawTextEx(defaultFont, line.c_str(), (Vector2){textBoxX + 10, textY}, contentFontSize, 2, BLACK);
            textY += contentFontSize + 5; // Adjust line spacing
        }

        // Allow scrolling with arrow keys
        if (IsKeyDown(KEY_DOWN)) {
            scrollOffset = std::min(scrollOffset + scrollSpeed, (int)(wrappedLines.size() * (contentFontSize + 5) - textBoxHeight));
        } else if (IsKeyDown(KEY_UP)) {
            scrollOffset = std::max(scrollOffset - scrollSpeed, 0);
        }
    }
    EndScissorMode();

    // Draw a copy button below the text box
    Rectangle copyButton = {
        (float)(screenW / 2 - 150),
        (float)(textBoxY + textBoxHeight + 20),
        300.f, 70.f
    };

    if (GuiButton(copyButton, "Copy to Clipboard")) {
        SetClipboardText(ocrText.c_str());
    }
}

