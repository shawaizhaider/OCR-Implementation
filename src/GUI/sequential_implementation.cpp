#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdlib.h>

#define MAX_FILEPATH_RECORDED 4096
#define MAX_FILEPATH_SIZE 2048

int main()
{
    // Initialization
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Image to Text Converter");
    Color greengrey = {155, 168, 171, 255};

    // Logo title settings
    const char *titleText = "Image to Text Converter";
    const int titleFontSize = 60;  
    const int titlePositionX = screenWidth / 2 - MeasureText(titleText, titleFontSize) / 2;
    const int titlePositionY = screenHeight / 4;

    // Button dimensions and positions
    const int buttonWidth = 300;
    const int buttonHeight = 70;
    const int buttonSpacing = 30;

    Rectangle uploadButton = {screenWidth / 2 - buttonWidth / 2, screenHeight / 2, buttonWidth, buttonHeight};
    Rectangle exitButton = {screenWidth / 2 - buttonWidth / 2, screenHeight / 2 + buttonHeight + buttonSpacing, buttonWidth, buttonHeight};

    // File drop variables
    int filePathCounter = 0;
    char *filePaths[MAX_FILEPATH_RECORDED] = {0}; // We will register a maximum of filepaths

    // Allocate space for the required file paths
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++)
    {
        filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
    }

    bool uploadMode = false;  // Flag to determine when to show file drop screen
    bool imageLoaded = false; // Flag to check if an image is loaded
    Image loadedImage = {0};  // Variable to store the loaded image
    Texture2D texture = {0};  // Variable to store the texture for drawing

    // Main loop
    while (!WindowShouldClose())
    {
        // Update
        if (!uploadMode)
        {
            if (CheckCollisionPointRec(GetMousePosition(), uploadButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                uploadMode = true; // Switch to file drop screen
            }

            if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                CloseWindow();
                break;
            }
        }
        else
        {
            if (IsFileDropped())
            {
                FilePathList droppedFiles = LoadDroppedFiles();

                for (int i = 0, offset = filePathCounter; i < (int)droppedFiles.count; i++)
                {
                    if (filePathCounter < (MAX_FILEPATH_RECORDED - 1))
                    {
                        TextCopy(filePaths[offset + i], droppedFiles.paths[i]);
                        filePathCounter++;
                    }
                }

                UnloadDroppedFiles(droppedFiles); // Unload filepaths from memory
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(greengrey);

        // Draw a dotted background
        for (int y = 0; y < screenHeight; y += 20)
        {
            for (int x = 0; x < screenWidth; x += 20)
            {
                DrawCircle(x, y, 2, Fade(DARKGRAY, 0.2f)); // Small semi-transparent dots
            }
        }

        if (!uploadMode)
        {
            // Draw title
            DrawText(titleText, titlePositionX, titlePositionY, titleFontSize, DARKGRAY);
            
            // Draw blurred background around buttons
            Rectangle blurArea = {
                screenWidth / 2 - buttonWidth / 2 - 50,
                screenHeight / 2 - buttonHeight - buttonSpacing / 2+35,
                buttonWidth + 100,
                buttonHeight * 2 + buttonSpacing + 100
            };
            for (int i = 0; i < 7; i++)
            {
                DrawRectangleRounded(blurArea, 0.3f, 12, Fade(WHITE, 0.1f - i * 0.015f));
            }

            // Draw buttons
            GuiSetStyle(DEFAULT, TEXT_SIZE, 30);    
            GuiButton(uploadButton, "Upload Image");
            GuiButton(exitButton, "Exit");
        }
        else
        {
            if (filePathCounter == 0)
            {
                DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
            }
            else
            {
                DrawText("Dropped files:", 100, 40, 20, DARKGRAY);

                for (int i = 0; i < filePathCounter; i++)
                {
                    if (i % 2 == 0)
                        DrawRectangle(0, 85 + 40 * i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                    else
                        DrawRectangle(0, 85 + 40 * i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));

                    DrawText(filePaths[i], 120, 100 + 40 * i, 10, GRAY);
                }

                // Add the "Load Image" button
                Rectangle loadButton = {screenWidth / 2 - buttonWidth / 2, (float)(85 + 40 * filePathCounter + buttonSpacing), buttonWidth, buttonHeight};
                if (GuiButton(loadButton, "Load Image"))
                {
                    if (filePathCounter > 0)
                    {
                        // Load the image from the first file in the list (you can choose a different one if needed)
                        loadedImage = LoadImage(filePaths[0]);
                        texture = LoadTextureFromImage(loadedImage); // Convert image to texture for drawing
                        imageLoaded = true;                          // Set flag to true
                    }
                }
            }

            // If the image is loaded, scale and draw it
            if (imageLoaded)
            {
                // Calculate the scaling factor to fit the image within the screen size
                float scaleX = (float)screenWidth / texture.width;
                float scaleY = (float)screenHeight / texture.height;
                float scale = (scaleX < scaleY) ? scaleX : scaleY; // Maintain aspect ratio

                // Calculate the width and height based on the scaling factor
                int width = (int)(texture.width * scale);
                int height = (int)(texture.height * scale);

                // Center the image
                int posX = (screenWidth - width) / 2;
                int posY = (screenHeight - height) / 2;

                // Draw the image with scaling
                DrawTextureEx(texture, (Vector2){(float)posX, (float)posY}, 0.0f, scale, WHITE);
            }
        }

        EndDrawing();
    }

    // Cleanup
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++)
    {
        RL_FREE(filePaths[i]); // Free allocated memory for all filepaths
    }
    if (imageLoaded)
    {
        UnloadImage(loadedImage); // Unload the image from memory
        UnloadTexture(texture);   // Unload the texture from memory
    }

    CloseWindow();
    return 0;
}
