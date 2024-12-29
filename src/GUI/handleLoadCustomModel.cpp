#include "../../include/GUI/interface.hpp"
#include "../../include/neuralNetwork.hpp"
#include <fstream>
#include <raylib.h>
#include "../../include/GUI/tinyfiledialogs.h"
#include <chrono>
#include <thread>
#include "../../include/utils/GUI_utils.hpp"


void handleLoadCustomModel() {
    static bool modelLoaded = false;
    static bool showModelDetails = false;
    static const char* statusMessage = NULL;
    static Color statusColor = BLACK;
    static neuralNetwork* currentModel = nullptr;
    static vector<int> modelTopology;
    static string currentWeightsFile;
    static vector<vector<double>> testData;
    static vector<vector<double>> labelData;
    // New static variables for test results
    static vector<string> testResults;
    static int currentTestSample = 0;
    static bool showTestResults = false;
    static int scrollOffset = 0;
    static bool isLoading = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!showModelDetails) {
        const char* instructions = "Click the button to select a model configuration file";
        int textWidth = MeasureText(instructions, 20);
        
        // Increased button width to fit text
        Rectangle selectButton = { 
            (float)(screenWidth/2 - 200),  // Wider button
            (float)(screenHeight/2 - 30),
            400,  // Increased width
            60 
        };

        // Center the button text
        if (GuiButton(selectButton, "Select Model Configuration")) {
            isLoading = true;
            
            // Clear the screen and draw loading state away from button
            BeginDrawing();
            ClearBackground(RAYWHITE);
            const char* loadingText = "Loading configuration...";
            DrawText(loadingText,
                    screenWidth/2 - MeasureText(loadingText, 30)/2,
                    screenHeight/2 - 150,  // Move text up
                    30, DARKGRAY);
            DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);  // Move spinner up
            EndDrawing();

            ShowTerminalProgress("Waiting for file selection");
            
            const char* filters[1] = { "*.json" };
            const char* selectedFile = tinyfd_openFileDialog(
                "Select Model Configuration",
                "",
                1,
                filters,
                "JSON Files",
                0
            );
            
            if (selectedFile != NULL) {
                try {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText("Processing configuration file...",
                            screenWidth/2 - MeasureText("Processing configuration file...", 30)/2,
                            screenHeight/2 - 150,
                            30, DARKGRAY);
                    DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);
                    EndDrawing();

                    ShowTerminalProgress("Reading configuration file");
                    ifstream configFile(selectedFile);
                    if (!configFile.is_open()) throw runtime_error("Failed to open configuration file");

                    string str((istreambuf_iterator<char>(configFile)), istreambuf_iterator<char>());
                    auto config = json::parse(str);

                    ShowTerminalProgress("Loading model files");
                    string testFile = config["testFile"];
                    string weightsFile = config["weightsFile"];
                    string labelsFile = config["labelsFile"];
                    cout<<"Weights File: " << weightsFile << endl;
                
                    ShowTerminalProgress("Initializing neural network");
                    vector<int> topology = config["topology"].get<vector<int>>();
                    neuralNetwork *nn = new neuralNetwork(topology);

                    ShowTerminalProgress("Loading test data");
                    vector< vector<double>> tests = readData::readCSV::fetchData(testFile);
                    vector< vector<double>> labels = readData::readCSV::fetchData(labelsFile);
                
                    cout << "Test samples: " << tests.size() 
                        << ", Features per sample: " 
                        << (tests.empty() ? 0 : tests[0].size()) << endl; 
                
                    // Add progress indicators
                    DrawText("Loading model...", 
                            screenWidth/2 - MeasureText("Loading model...", 20)/2,
                            screenHeight/2 + 100,
                            20,
                            DARKGRAY);
                    EndDrawing();
                    BeginDrawing();

                    ShowTerminalProgress("Loading model weights");
                    nn->loadModel(weightsFile);
                    cout << "Model loaded successfully!" << endl;
                
                    ShowTerminalProgress("Performing initial tests");
                    for (int i = 0; i < tests.size(); i++) {
                        nn->setCurrentInput(tests[i]);
                        nn->setTargets(labels[i]);
                        nn->feedForward();
                        nn->setErrors();
                        nn->printInput();
                        nn->printOutput();
                        nn->printTargets();
                        cout << "Error at example "<< i << ": " << nn->getGlobalError() << endl;
                    }
                
                    nn -> printNetwork();

                    // Store model details for visualization
                    modelTopology = topology;
                    currentWeightsFile = weightsFile;
                    testData = tests;
                    labelData = labels;
                    
                    if (currentModel != nullptr) {
                        delete currentModel;
                    }
                    currentModel = nn; // Don't delete nn here anymore
                    
                    statusMessage = TextFormat("Model loaded successfully from %s!", weightsFile.c_str());
                    statusColor = GREEN;
                    modelLoaded = true;
                    showModelDetails = true;

                    isLoading = false;
                    
                } catch (const exception& e) {
                    cout << "\rError: " << e.what() << "                 " << endl;
                    isLoading = false;
                    statusMessage = TextFormat("Error: %s", e.what());
                    statusColor = RED;
                    modelLoaded = false;
                }
            } else {
                cout << "\rFile selection cancelled                 " << endl;
                isLoading = false;
            }
        }

        // Draw loading state only if not drawing anything else
        if (isLoading) {
            DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);
            // Add small delay to prevent GUI freeze
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Draw instructions with proper centering
        if (!modelLoaded) {
            DrawText(instructions,
                    screenWidth/2 - textWidth/2,
                    screenHeight/2 - 100,
                    20,
                    DARKGRAY);
        }

        // Draw status message if exists
        if (statusMessage) {
            DrawText(statusMessage, 
                    screenWidth/2 - MeasureText(statusMessage, 20)/2, 
                    screenHeight/2 + 50, 
                    20, 
                    statusColor);
        }
    }
    else {
        // Model Details View
        const float padding = 20.0f;
        const float sectionHeight = 120.0f;  // Reduced base height to fit more on screen
        float currentY = padding;
        const float screenWidthf = (float)screenWidth;
        const char* arrow = "-->"; 

        // Network Parameters Section with dynamic height
        float boxWidth = screenWidthf - 40.0f;
        float textWidth = MeasureText(currentWeightsFile.c_str(), 20) + padding * 2;
        float requiredWidth = max(boxWidth, textWidth);

        DrawRectangleRounded(
            {10.0f, currentY, requiredWidth, sectionHeight * 1.2f},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        DrawText("Network Parameters", (int)padding, (int)(currentY + 10), 24, DARKGRAY);
        DrawText(TextFormat("Layers: %d", modelTopology.size()), 
                (int)(padding + 20), (int)(currentY + 50), 20, BLACK);
        DrawText(TextFormat("Weights File: %s", currentWeightsFile.c_str()), 
                (int)(padding + 20), (int)(currentY + 85), 20, BLACK);
        currentY += sectionHeight * 1.2f + padding;

        // Topology Visualization with ASCII arrow
        DrawRectangleRounded(
            {10.0f, currentY, boxWidth, sectionHeight * 1.2f},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        DrawText("Network Topology", (int)padding, (int)(currentY + 10), 24, DARKGRAY);
        
        // Build topology string with ASCII arrow
        string topology = "Neurons per layer: ";
        for (size_t i = 0; i < modelTopology.size(); i++) {
            topology += TextFormat("%d", modelTopology[i]);
            if (i < modelTopology.size() - 1) topology += " " + string(arrow) + " ";
        }
        DrawText(topology.c_str(), (int)(padding + 20), (int)(currentY + 60), 20, BLACK);
        currentY += sectionHeight * 1.2f + padding;

        // Test Data Info with larger box
        DrawRectangleRounded(
            {10.0f, currentY, boxWidth, sectionHeight * 1.2f},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        DrawText("Test Data Information", (int)padding, (int)(currentY + 10), 24, DARKGRAY);
        DrawText(TextFormat("Test Samples: %d", testData.size()), 
                (int)(padding + 20), (int)(currentY + 50), 20, BLACK);
        DrawText(TextFormat("Features per sample: %d", testData.empty() ? 0 : testData[0].size()), 
                (int)(padding + 20), (int)(currentY + 85), 20, BLACK);
        currentY += sectionHeight * 1.2f + padding;

        // Interactive Test Section with larger visible area
        float testSectionHeight = (float)screenHeight - currentY - padding * 4;  // Use remaining screen space
        DrawRectangleRounded(
            {10.0f, currentY, screenWidthf - 20.0f, testSectionHeight},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        DrawText("Model Testing", (int)padding, (int)(currentY + 10), 24, DARKGRAY);

        // Control buttons with proper sizing and spacing
        Rectangle testButton = {
            padding + 20.0f,
            currentY + 40.0f,
            300.0f,  // Fixed wider width
            50.0f    // Taller height for better visibility
        };
        
        Rectangle nextButton = {
            padding + 340.0f,  // Fixed position with good gap
            currentY + 40.0f,
            300.0f,  // Match width
            50.0f    // Match height
        };

        if (GuiButton(testButton, "Run Test Sample")) {
            if (!testData.empty() && currentModel != nullptr) {
                testResults.clear();
                currentModel->setCurrentInput(testData[currentTestSample]);
                currentModel->setTargets(labelData[currentTestSample]);
                currentModel->feedForward();
                currentModel->setErrors();
                
                // Capture results as strings
                stringstream ss;
                ss << "Test Sample " << currentTestSample + 1 << ":\n";
                ss << "Input: ";
                for (double val : testData[currentTestSample]) {
                    ss << val << " ";
                }
                ss << "\nOutput: ";
                vector<double> output = currentModel->getOutput();
                for (double val : output) {
                    ss << val << " ";
                }
                ss << "\nTarget: ";
                for (double val : labelData[currentTestSample]) {
                    ss << val << " ";
                }
                ss << "\nError: " << currentModel->getGlobalError();
                testResults.push_back(ss.str());
                showTestResults = true;
            }
        }

        if (GuiButton(nextButton, "Next Sample")) {
            if (currentTestSample < testData.size() - 1) {
                currentTestSample++;
            } else {
                currentTestSample = 0;
            }
        }

        // Test Results Display with constrained scrolling
        if (showTestResults && !testResults.empty()) {
            float resultsY = currentY + 100.0f;
            float resultsHeight = min(testSectionHeight - 170.0f, 400.0f);  // Constrain max height
            
            // Results background
            DrawRectangleRounded(
                {10.0f, resultsY, boxWidth, resultsHeight},
                0.1f, 10, Fade(WHITE, 0.95f));
            
            // Scrolling logic with constraints
            int lineHeight = 30;  // Increased for better readability
            int visibleLines = (int)(resultsHeight / lineHeight);
            int totalContentHeight = testResults.size() * lineHeight;
            int maxScroll = max(0, totalContentHeight - (int)resultsHeight);
            
            scrollOffset -= (int)(GetMouseWheelMove() * 60);
            scrollOffset = std::max(0, std::min(scrollOffset, maxScroll));

            // Constrained scissor mode
            BeginScissorMode(
                (int)(padding + 10), 
                (int)resultsY, 
                (int)(boxWidth - padding * 4), // More space for scrollbar
                (int)resultsHeight
            );

            float textY = resultsY + padding - scrollOffset;
            for (const string& result : testResults) {
                DrawText(result.c_str(), (int)(padding + 20), (int)textY, 20, BLACK);
                textY += 30;  // Increased line spacing
            }
            EndScissorMode();

            // Draw scroll bar if needed
            if (maxScroll > 0) {
                // Scroll track
                DrawRectangle(
                    (int)(boxWidth - 20), 
                    (int)resultsY,
                    10,
                    (int)resultsHeight,
                    Fade(GRAY, 0.2f));

                // Scroll handle
                float scrollPercent = (float)scrollOffset / maxScroll;
                float handleHeight = (resultsHeight / totalContentHeight) * resultsHeight;
                float handleY = resultsY + scrollPercent * (resultsHeight - handleHeight);
                
                DrawRectangleRounded(
                    {boxWidth - 20, handleY, 10, handleHeight},
                    0.5f, 2, Fade(GRAY, 0.8f));

                // Scroll hints
                if (scrollOffset > 0) {
                    DrawText("▲", (int)(boxWidth - 25), (int)resultsY, 20, Fade(DARKGRAY, 0.7f));
                }
                if (scrollOffset < maxScroll) {
                    DrawText("▼", (int)(boxWidth - 25), (int)(resultsY + resultsHeight - 20), 20, Fade(DARKGRAY, 0.7f));
                }
            }
        }

        // Single navigation button at the bottom right
        Rectangle backButton = {
            screenWidthf - 220.0f,
            (float)screenHeight - 70.0f,
            200.0f,
            50.0f
        };

        if (GuiButton(backButton, "Back")) {
            showModelDetails = false;
            showTestResults = false;
            testResults.clear();
            currentTestSample = 0;
            scrollOffset = 0;
            if (currentModel != nullptr) {
                delete currentModel;
                currentModel = nullptr;
            }
            modelLoaded = false;
        }
    }
}