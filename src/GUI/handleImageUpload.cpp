#include "../../include/GUI/interface.hpp"
#include "../../include/utils/GUI_utils.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <raylib.h>
#include "../../include/GUI/raygui.h"
#include <tesseract/baseapi.h>

using namespace cv;
using namespace cv::dnn;
using namespace std;

// Remove HandwritingRecognizer class definition (now in separate files)

void handleImageUpload(bool &imageLoaded, bool &ocrMode, Image &loadedImage, 
                      Texture2D &texture, std::string &ocrText, 
                      char *filePaths[], int &filePathCounter) {
    static bool isProcessing = false;
    const float padding = 20.0f;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (filePathCounter == 0) {
        // Draw drop zone
        Rectangle dropZone = {
            (float)screenWidth/4,
            (float)screenHeight/4,
            (float)screenWidth/2,
            (float)screenHeight/2
        };

        Color faintGray = { 80, 80, 80, 128 };
        DrawRectangleRoundedLines(dropZone, 0.1f, 20, faintGray);  // Using raylib's function
        DrawText("Drop your image file here",
                screenWidth/2 - MeasureText("Drop your image file here", 30)/2,
                screenHeight/2 - 15,
                30, Fade(DARKGRAY, 0.7f));

        // Handle file drop
        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            if (droppedFiles.count > 0) {
                const char* imagePath = droppedFiles.paths[0];
                // Process dropped image
                processImage(imagePath, imageLoaded, loadedImage, texture, ocrText, filePaths);
                filePathCounter = 1;
            }
            UnloadDroppedFiles(droppedFiles);
        }
    } else {
        // Display loaded image
        if (imageLoaded) {
            const float maxPreviewHeight = screenHeight * 0.6f;
            Rectangle previewArea = {
                padding,
                padding,
                (float)screenWidth - padding * 2,
                maxPreviewHeight
            };

            drawImageWithScaling(texture, loadedImage, previewArea);
        }

        // Control buttons
        Rectangle processButton = {
            (float)screenWidth/2 - 150,
            (float)screenHeight - 140,
            300, 60
        };

        Rectangle clearButton = {
            padding,
            (float)screenHeight - 140,
            150, 60
        };

        if (GuiButton(processButton, "Process Handwriting")) {
            if (imageLoaded) {
                processOCR(filePaths[0], ocrText, ocrMode);
            }
        }

        if (GuiButton(clearButton, "Clear")) {
            clearResources(imageLoaded, loadedImage, texture, filePathCounter);
        }
    }

    // Show processing indicator
    if (isProcessing) {
        DrawLoadingSpinner(screenWidth/2, screenHeight/2 + 50);
    }
}

// Helper functions
void processImage(const char* imagePath, bool &imageLoaded, Image &loadedImage, 
                 Texture2D &texture, string &ocrText, char *filePaths[]) {
    Mat img = imread(imagePath, IMREAD_COLOR);
    if (!img.empty()) {
        try {
            cout << "Image loaded: " << img.size() << " channels: " << img.channels() << endl;
            
            // Convert to RGB and ensure continuous memory
            Mat processed;
            cvtColor(img, processed, COLOR_BGR2RGB);
            
            // Create continuous memory block
            processed = processed.clone();
            
            // Create Image directly
            loadedImage = LoadImage(imagePath);
            
            if (loadedImage.data != NULL) {
                texture = LoadTextureFromImage(loadedImage);
                imageLoaded = true;
                TextCopy(filePaths[0], imagePath);
                cout << "Image processed successfully" << endl;
            } else {
                cout << "Failed to load image directly, trying alternative method..." << endl;
                
                // Fallback method using raw data
                unsigned char* imageData = new unsigned char[processed.total() * 3];
                memcpy(imageData, processed.data, processed.total() * 3);
                
                Image temp = {
                    .data = imageData,
                    .width = processed.cols,
                    .height = processed.rows,
                    .mipmaps = 1,
                    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8
                };
                
                loadedImage = temp;
                texture = LoadTextureFromImage(loadedImage);
                imageLoaded = true;
                TextCopy(filePaths[0], imagePath);
                
                delete[] imageData;
                cout << "Image processed successfully using fallback method" << endl;
            }
        } catch (const exception& e) {
            cerr << "Error processing image: " << e.what() << endl;
        }
    } else {
        cerr << "Failed to load image: " << imagePath << endl;
    }
}

void preprocessImage(const Mat& input, Mat& output) {
    try {
        // Ensure consistent input
        Mat temp;
        if (input.channels() == 3) {
            cvtColor(input, temp, COLOR_BGR2GRAY);
        } else {
            temp = input.clone();
        }
        
        // Basic preprocessing
        GaussianBlur(temp, temp, Size(3,3), 0);
        
        // Adaptive thresholding with more conservative parameters
        adaptiveThreshold(temp, output, 255,
                         ADAPTIVE_THRESH_GAUSSIAN_C,
                         THRESH_BINARY, 11, 5);
                         
        cout << "Preprocessing completed. Output size: " << output.size() << endl;
    } catch (const exception& e) {
        cerr << "Preprocessing error: " << e.what() << endl;
        output = input.clone(); // Fallback to original image
    }
}

void drawImageWithScaling(const Texture2D& texture, const Image& image, const Rectangle& area) {
    float scale = min(
        area.width / image.width,
        area.height / image.height
    );

    float scaledWidth = image.width * scale;
    float scaledHeight = image.height * scale;
    
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    Rectangle destRect = {
        area.x + (area.width - scaledWidth) / 2,
        area.y + (area.height - scaledHeight) / 2,
        scaledWidth,
        scaledHeight
    };
    
    // Calculate padding based on image size
    float padding = min(20.0f, min(scaledWidth, scaledHeight) * 0.05f);
    
    // Draw background box with dynamic padding
    DrawRectangleRounded(
        {destRect.x - padding, 
         destRect.y - padding, 
         scaledWidth + padding * 2, 
         scaledHeight + padding * 2},
        0.1f, 
        10, 
        Fade(LIGHTGRAY, 0.3f)
    );
    
    DrawTexturePro(texture, sourceRect, destRect, (Vector2){0, 0}, 0, WHITE);
}

void processOCR(const char* imagePath, string& ocrText, bool& ocrMode) {
    try {
        // Initialize Tesseract
        tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
        if (ocr->Init(NULL, "eng")) {
            throw runtime_error("Could not initialize tesseract.");
        }

        // Load image and process
        Mat img = imread(imagePath, IMREAD_COLOR);
        if (img.empty()) {
            throw runtime_error("Failed to load image");
        }

        // Preprocess
        Mat processed;
        preprocessImage(img, processed);

        cout << "OCR Processing image: " << processed.size() << " channels: " << processed.channels() << endl;

        // Set image data
        ocr->SetImage(processed.data, processed.cols, processed.rows, 
                     processed.channels(), processed.step);

        // Get OCR result
        char* outText = ocr->GetUTF8Text();
        if (outText != nullptr) {
            ocrText = string(outText);
            delete[] outText;
            
            if (ocrText.empty()) {
                ocrText = "No text detected";
            }
        } else {
            ocrText = "OCR failed to process image";
        }

        // Cleanup
        ocr->End();
        delete ocr;

        ocrMode = true;
        cout << "OCR Processing completed. Result: " << ocrText << endl;
    } catch (const exception& e) {
        cerr << "OCR processing error: " << e.what() << endl;
        ocrText = "Error: " + string(e.what());
    }
}

void clearResources(bool& imageLoaded, Image& loadedImage, Texture2D& texture, int& filePathCounter) {
    if (imageLoaded) {
        UnloadImage(loadedImage);
        UnloadTexture(texture);
        imageLoaded = false;
    }
    filePathCounter = 0;
}
