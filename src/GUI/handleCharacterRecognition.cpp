#include "../../include/GUI/interface.hpp"
#include "../../include/utils/GUI_utils.hpp"
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <raylib.h>
#include <exception>
#include "../../include/GUI/tinyfiledialogs.h"

using namespace std;

using namespace cv;

void handleCharacterRecognition() {
    static bool imageLoaded = false;
    static bool showResults = false;
    static const char* statusMessage = NULL;
    static Color statusColor = BLACK;
    static Image loadedImage;
    static Texture2D texture;
    static string recognizedText;
    static bool isProcessing = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!showResults) {
        const char* instructions = "Click the button to select an image for character recognition";
        
        Rectangle selectButton = { 
            (float)(screenWidth/2 - 250),
            (float)(screenHeight/2 - 35),
            500, 70 
        };

        if (GuiButton(selectButton, "Select Image")) {
            isProcessing = true;
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Processing image...", 
                    screenWidth/2 - MeasureText("Processing image...", 30)/2,
                    screenHeight/2 - 150, 30, DARKGRAY);
            DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);
            EndDrawing();

            const char* patterns[] = {
                "*.png",
                "*.jpg",
                "*.jpeg",
                "*.bmp"
            };
            const char* selectedFile = tinyfd_openFileDialog(
                "Select Image",
                "",
                4,  // number of filter patterns
                patterns,
                "Image Files (*.png, *.jpg, *.jpeg, *.bmp)",
                0
            );

            if (selectedFile != NULL) {
                try {
                    Mat img = imread(selectedFile);
                    if (img.empty()) throw runtime_error("Failed to load image");

                    Mat gray, binary;
                    cvtColor(img, gray, COLOR_BGR2GRAY);
                    threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

                    // Initialize Tesseract OCR
                    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
                    if (ocr->Init(NULL, "eng")) {
                        throw runtime_error("Could not initialize tesseract");
                    }

                    ocr->SetImage(binary.data, binary.cols, binary.rows, 1, binary.step);
                    
                    // Get OCR result
                    char* outText = ocr->GetUTF8Text();
                    recognizedText = string(outText);
                    
                    // Cleanup
                    delete[] outText;
                    ocr->End();
                    delete ocr;

                    // Convert OpenCV Mat to Raylib Image/Texture
                    Image temp = {
                        .data = img.data,
                        .width = img.cols,
                        .height = img.rows,
                        .mipmaps = 1,
                        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8
                    };
                    loadedImage = LoadImageFromMemory(".png", (unsigned char*)img.data, img.total() * img.channels());
                    texture = LoadTextureFromImage(loadedImage);

                    imageLoaded = true;
                    showResults = true;
                    statusMessage = "Image processed successfully!";
                    statusColor = GREEN;

                } catch (const exception& e) {
                    statusMessage = TextFormat("Error: %s", e.what());
                    statusColor = RED;
                }
            }
            isProcessing = false;
        }
        DrawText(instructions,
                screenWidth/2 - MeasureText(instructions, 20)/2,
                screenHeight/2 - 100, 20, DARKGRAY);

        if (statusMessage) {
            DrawText(statusMessage,
                    screenWidth/2 - MeasureText(statusMessage, 20)/2,
                    screenHeight/2 + 50, 20, statusColor);
        }

        if (isProcessing) {
            DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);
        }
    }
    else {
        const float padding = 20.0f;
        // Image display section with proper sizing
        if (imageLoaded) {
            const float maxImageHeight = (float)screenHeight * 0.4f;  // 40% of screen height
            float scale = min(
                (float)(screenWidth - padding * 2) / texture.width,
                maxImageHeight / texture.height
            );

            float scaledWidth = texture.width * scale;
            float scaledHeight = texture.height * scale;
            
            // Center the image horizontally
            Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
            Rectangle destRect = {
                (screenWidth - scaledWidth) / 2,  // Center horizontally
                padding,                          // Top padding
                scaledWidth,
                scaledHeight
            };
            
            // Fix Fade calls by creating proper Color structures
            DrawRectangleRounded(
                {destRect.x - 10, destRect.y - 10, scaledWidth + 20, scaledHeight + 20},
                0.1f, 10, Fade((Color){211, 211, 211, 255}, 0.3f));  // LIGHTGRAY with alpha
            
            DrawTexturePro(texture, sourceRect, destRect, (Vector2){0, 0}, 0, WHITE);
        }

        // Recognized text section with scrolling
        static int scrollOffset = 0;
        Rectangle textBox = {
            padding,
            (float)screenHeight * 0.45f,  // Start below image
            (float)screenWidth - padding * 2,
            (float)screenHeight * 0.45f    // Use remaining space minus buttons
        };

        DrawRectangleRounded(textBox, 0.1f, 10, Fade((Color){211, 211, 211, 255}, 0.3f));

        DrawText("Recognized Text:", 
                (int)textBox.x + 10, 
                (int)textBox.y + 10, 
                24, DARKGRAY);

        // Calculate text dimensions and scrolling
        const int lineHeight = 25;
        vector<string> lines;
        string currentLine;
        int maxWidth = textBox.width - padding * 3;

        // Split text into lines
        for (char c : recognizedText) {
            if (c == '\n' || MeasureText((currentLine + c).c_str(), 20) > maxWidth) {
                if (!currentLine.empty()) lines.push_back(currentLine);
                currentLine = (c == '\n' ? "" : string(1, c));
            } else {
                currentLine += c;
            }
        }
        if (!currentLine.empty()) lines.push_back(currentLine);

        // Handle scrolling
        int totalTextHeight = lines.size() * lineHeight;
        int visibleHeight = textBox.height - 70;  // Account for header
        int maxScroll = max(0, totalTextHeight - visibleHeight);
        
        scrollOffset -= (int)(GetMouseWheelMove() * 60);
        scrollOffset = std::max(0, std::min(scrollOffset, maxScroll));

        // Draw text with scrolling
        BeginScissorMode(
            textBox.x + 10,
            textBox.y + 50,
            textBox.width - 40,
            visibleHeight
        );

        float textY = textBox.y + 50 - scrollOffset;
        for (const string& line : lines) {
            DrawText(line.c_str(),
                    textBox.x + 20,
                    textY,
                    20, BLACK);
            textY += lineHeight;
        }
        EndScissorMode();

        // Draw scroll bar if needed
        if (maxScroll > 0) {
            float scrollBarHeight = (visibleHeight / totalTextHeight) * visibleHeight;
            float scrollBarY = textBox.y + 50 + 
                             ((float)scrollOffset / maxScroll) * (visibleHeight - scrollBarHeight);
            
            // Scroll track
            DrawRectangle(
                textBox.x + textBox.width - 20,
                textBox.y + 50,
                5,
                visibleHeight,
                Fade((Color){128, 128, 128, 255}, 0.2f));  // GRAY with alpha
            
            // Scroll handle
            DrawRectangleRounded(
                {textBox.x + textBox.width - 20,
                 scrollBarY,
                 5,
                 scrollBarHeight},
                1.0f, 2, Fade((Color){128, 128, 128, 255}, 0.8f));  // GRAY with alpha
        }

        // Control buttons in a row
        const float buttonY = screenHeight - padding - 50;
        Rectangle copyButton = {
            screenWidth/2 - 150,
            buttonY,
            300, 50
        };

        Rectangle backButton = {
            padding,
            buttonY,
            200, 50
        };

        if (GuiButton(copyButton, "Copy to Clipboard")) {
            SetClipboardText(recognizedText.c_str());
        }

        if (GuiButton(backButton, "Back")) {
            showResults = false;
            imageLoaded = false;
            UnloadTexture(texture);
            UnloadImage(loadedImage);
        }
    }
}
