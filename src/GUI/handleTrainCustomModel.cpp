// Project includes first
#include "../../include/GUI/interface.hpp"
#include "../../include/neuralNetwork.hpp"
#include "../../include/utils/GUI_utils.hpp"
#include "../../include/GUI/tinyfiledialogs.h"

// Standard library includes
#include <fstream>
#include <chrono>
#include <thread>
#include <streambuf>

using json = nlohmann::json;

// Progress bar drawing helper
void DrawProgressBar(float x, float y, float width, float height, float progress, const char* text) {
    DrawRectangleRounded({x, y, width, height}, 0.2f, 20, Fade(LIGHTGRAY, 0.4f));
    DrawRectangleRounded({x, y, width * progress, height}, 0.2f, 20, Fade(GREEN, 0.6f));
    DrawText(text, (int)(x + 10), (int)(y + height/4), 20, DARKGRAY);
}

void handleTrainCustomModel() {
    // Add new static variables for configuration display
    static double learningRateVal = 0.0;
    static double momentumVal = 0.0;
    static int maxEpochsVal = 0;
    static string trainingFilePath;
    static string labelsFilePath;
    static string weightsFilePath;
    static bool modelLoaded = false;
    static bool showTrainingProgress = false;
    static const char* statusMessage = NULL;
    static Color statusColor = BLACK;
    static neuralNetwork* currentModel = nullptr;
    static vector<int> topology;
    static float currentEpoch = 0;
    static float totalEpochs = 0;
    static float currentError = 0;
    static vector<float> errorHistory;
    static bool isTraining = false;
    static bool isLoading = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!showTrainingProgress) {
        const char* instructions = "Click the button to select a training configuration file";
        Rectangle selectButton = { 
            (float)(screenWidth/2 - 250),  // Increased width for longer text
            (float)(screenHeight/2 - 35),  // Adjusted position
            500,  // Wider button
            70    // Taller button
        };

        if (GuiButton(selectButton, "Select Training Configuration")) {
            isLoading = true;

            // Start continuous drawing loop
            while (isLoading) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                
                const char* loadingText = "Loading configuration...";
                DrawText(loadingText,
                        screenWidth/2 - MeasureText(loadingText, 30)/2,
                        screenHeight/2 - 150,
                        30, DARKGRAY);
                
                DrawLoadingSpinner(screenWidth/2, screenHeight/2 - 80);
                ShowTerminalProgress("Waiting for file selection");
                
                EndDrawing();

                // Check for file selection
                const char* filters[1] = { "*.json" };
                const char* selectedFile = tinyfd_openFileDialog(
                    "Select Training Configuration",
                    "",
                    1,
                    filters,
                    "JSON Files",
                    0
                );

                if (selectedFile != NULL) {
                    try {
                        ShowTerminalProgress("Reading configuration file");
                        cout << "Reading configuration file: " << selectedFile << endl;
                        
                        // Open and read file content
                        ifstream configFile(selectedFile);
                        if (!configFile.is_open()) {
                            throw runtime_error("Failed to open file: " + string(selectedFile));
                        }

                        // Read entire file into string using iterators
                        string str((istreambuf_iterator<char>(configFile)),
                                 istreambuf_iterator<char>());
                        configFile.close();

                        ShowTerminalProgress("Parsing configuration");
                        // Parse JSON with error checking
                        auto config = json::parse(str);

                        ShowTerminalProgress("Loading parameters");
                        // Validate required fields
                        if (!config.contains("learningRate")) throw runtime_error("Missing learningRate in config");
                        if (!config.contains("momentum")) throw runtime_error("Missing momentum in config");
                        if (!config.contains("maxEpochs")) throw runtime_error("Missing maxEpochs in config");
                        if (!config.contains("trainingSet")) throw runtime_error("Missing trainingSet in config");
                        if (!config.contains("labelsFile")) throw runtime_error("Missing labelsFile in config");
                        if (!config.contains("weightsFile")) throw runtime_error("Missing weightsFile in config");
                        if (!config.contains("topology")) throw runtime_error("Missing topology in config");

                        // Extract configuration with type checking
                        learningRateVal = config["learningRate"].get<double>();
                        momentumVal = config["momentum"].get<double>();
                        maxEpochsVal = config["maxEpochs"].get<int>();
                        trainingFilePath = config["trainingSet"].get<string>();
                        labelsFilePath = config["labelsFile"].get<string>();
                        weightsFilePath = config["weightsFile"].get<string>();
                        topology = config["topology"].get<vector<int>>();

                        ShowTerminalProgress("Initializing neural network");
                        cout << "Configuration loaded:" << endl
                             << "Learning Rate: " << learningRateVal << endl
                             << "Momentum: " << momentumVal << endl
                             << "Max Epochs: " << maxEpochsVal << endl
                             << "Training File: " << trainingFilePath << endl
                             << "Labels File: " << labelsFilePath << endl
                             << "Weights File: " << weightsFilePath << endl;

                        cout << "Loading training data..." << endl;
                        vector<vector<double>> trainingData = readData::readCSV::fetchData(trainingFilePath);
                        vector<vector<double>> labels = readData::readCSV::fetchData(labelsFilePath);

                        // Validate data dimensions
                        if (trainingData.empty() || labels.empty()) {
                            throw runtime_error("Empty training data or labels");
                        }

                        for (int i = 0; i < trainingData.size(); i++) {
                            if (trainingData[i].size() != topology[0]) {
                                throw runtime_error("Training data dimension mismatch");
                            }
                            if (labels[i].size() != topology.back()) {
                                throw runtime_error("Labels dimension mismatch");
                            }
                        }

                        if (currentModel != nullptr) {
                            delete currentModel;
                        }

                        currentModel = new neuralNetwork(topology);
                        totalEpochs = maxEpochsVal;
                        errorHistory.clear();
                        showTrainingProgress = true;
                        isTraining = true;

                        // Start training loop in a separate thread
                        std::thread([=]() {
                            for(int epoch = 0; epoch < maxEpochsVal && isTraining; epoch++) {
                                float epochError = 0;
                                for(int j = 0; j < trainingData.size(); j++) {
                                    currentModel->setCurrentInput(trainingData[j]);
                                    currentModel->setTargets(labels[j]);
                                    currentModel->feedForward();
                                    currentModel->setErrors();
                                    currentModel->backPropagate();
                                    epochError += currentModel->getGlobalError();
                                }
                                currentEpoch = epoch + 1;
                                currentError = epochError / trainingData.size();
                                errorHistory.push_back(currentError);
                            }
                            
                            if (isTraining) {
                                currentModel->saveModel(weightsFilePath);
                                statusMessage = "Training completed successfully!";
                                statusColor = GREEN;
                            }
                            isTraining = false;
                        }).detach();

                        cout << "\rConfiguration loaded successfully!" << endl;
                        isLoading = false;

                    } catch (const json::parse_error& e) {
                        cout << "JSON parsing error: " << e.what() << endl;
                        statusMessage = TextFormat("JSON Error: Invalid format in config file");
                        statusColor = RED;
                        isLoading = false;
                        isTraining = false;
                    } catch (const json::type_error& e) {
                        cout << "JSON type error: " << e.what() << endl;
                        statusMessage = TextFormat("JSON Error: Invalid data type in config");
                        statusColor = RED;
                        isLoading = false;
                        isTraining = false;
                    } catch (const exception& e) {
                        statusMessage = TextFormat("Error: %s", e.what());
                        statusColor = RED;
                        isLoading = false;
                        isTraining = false;
                    }
                } else {
                    cout << "\rFile selection cancelled" << endl;
                    isLoading = false;
                }
            }
        }

        // Draw instructions and status
        DrawText(instructions,
                screenWidth/2 - MeasureText(instructions, 20)/2,
                screenHeight/2 - 100,
                20, DARKGRAY);

        if (statusMessage) {
            DrawText(statusMessage,
                    screenWidth/2 - MeasureText(statusMessage, 20)/2,
                    screenHeight/2 + 50,
                    20, statusColor);
        }
    }
    else {
        // Training progress view
        const float padding = 20.0f;
        
        // Configuration Info Section
        DrawRectangleRounded(
            {10, padding, (float)screenWidth - 20, 250},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        
        DrawText("Training Configuration", (int)padding + 10, (int)padding + 10, 24, DARKGRAY);
        DrawText(TextFormat("Learning Rate: %.4f", learningRateVal), 
                (int)padding + 20, (int)padding + 45, 20, BLACK);
        DrawText(TextFormat("Momentum: %.4f", momentumVal), 
                (int)padding + 20, (int)padding + 70, 20, BLACK);
        DrawText(TextFormat("Max Epochs: %d", maxEpochsVal), 
                (int)padding + 20, (int)padding + 95, 20, BLACK);
        
        // File paths (with truncation if too long)
        const int maxPathWidth = screenWidth - 200;
        string trainFile = "Training File: " + trainingFilePath;
        string labelFile = "Labels File: " + labelsFilePath;
        string weightFile = "Weights File: " + weightsFilePath;
        
        DrawText(TextFormat("%s", trainFile.c_str()), 
                (int)padding + 20, (int)padding + 120, 20, BLACK);
        DrawText(TextFormat("%s", labelFile.c_str()), 
                (int)padding + 20, (int)padding + 145, 20, BLACK);
        DrawText(TextFormat("%s", weightFile.c_str()), 
                (int)padding + 20, (int)padding + 170, 20, BLACK);

        // Training Progress Section (moved down)
        float progressY = padding + 270;
        DrawRectangleRounded(
            {10, progressY, (float)screenWidth - 20, 200},
            0.1f, 10, Fade(LIGHTGRAY, 0.3f));
        
        // Progress bar
        float progress = currentEpoch / totalEpochs;
        char progressText[32];
        sprintf(progressText, "Epoch: %.0f/%.0f", currentEpoch, totalEpochs);
        DrawProgressBar(padding + 10, progressY + 50, screenWidth - 60, 40, progress, progressText);

        // Current error with more precision
        char errorText[64];
        sprintf(errorText, "Current Error: %.8f", currentError);
        DrawText(errorText, padding + 10, progressY + 100, 20, DARKGRAY);

        // Error graph relabeled as Accuracy
        if (!errorHistory.empty()) {
            const float graphHeight = 300;
            const float graphWidth = screenWidth - 40;
            const float graphY = progressY + 220;

            DrawRectangleRounded(
                {10, graphY, graphWidth, graphHeight},
                0.1f, 10, Fade(LIGHTGRAY, 0.3f));
            
            DrawText("Training Accuracy Over Time", padding + 10, graphY + 10, 20, DARKGRAY);

            // Find error range with proper scaling
            float minError = *min_element(errorHistory.begin(), errorHistory.end());
            float maxError = *max_element(errorHistory.begin(), errorHistory.end());
            
            // Add small epsilon to prevent log(0) and ensure proper scaling
            const float epsilon = 1e-10f;
            minError = log10(minError + epsilon);
            maxError = log10(maxError + epsilon);
            float errorRange = maxError - minError;

            // Draw grid lines from max to min (top to bottom)
            const int numGridLines = 5;
            for (int i = 0; i <= numGridLines; i++) {
                float y = graphY + 40 + (i * (graphHeight - 80) / numGridLines);
                float errorValue = pow(10, maxError - (i * errorRange / numGridLines));
                DrawText(TextFormat("%.6f", errorValue), 
                        padding + 10, y - 10, 16, Fade(DARKGRAY, 0.7f));
                DrawLine((int)(padding + 100), (int)y, 
                        (int)(graphWidth - 20), (int)y, 
                        Fade(DARKGRAY, 0.2f));
            }
            
            // Draw error curve (decreasing from top to bottom)
            Vector2 *points = new Vector2[errorHistory.size()];
            for (size_t i = 0; i < errorHistory.size(); i++) {
                float x = padding + 120 + i * (graphWidth - 140) / (errorHistory.size() - 1);
                float errorVal = log10(errorHistory[i] + epsilon);
                // Invert Y position so larger errors are at top
                float normalizedError = (maxError - errorVal) / errorRange;
                float y = graphY + 40 + normalizedError * (graphHeight - 80);
                points[i] = {x, y};
            }

            // Draw curve with lines
            for (size_t i = 1; i < errorHistory.size(); i++) {
                DrawLineV(points[i-1], points[i], RED);
            }
            delete[] points;

            // Draw axes labels
            DrawText("Accuracy (log scale)", padding + 10, graphY + 30, 16, DARKGRAY);
            DrawText("Training Progress →", 
                    padding + graphWidth/2 - 50, 
                    graphY + graphHeight - 20, 16, DARKGRAY);

            // Add min/max indicators
            DrawText("High", padding + 10, graphY + 40, 14, DARKGRAY);
            DrawText("Low", padding + 10, graphY + graphHeight - 40, 14, DARKGRAY);
        }

        // Control buttons
        Rectangle stopButton = {
            (float)screenWidth - 220,
            (float)screenHeight - 70,
            200, 50
        };

        if (GuiButton(stopButton, isTraining ? "Stop Training" : "Back")) {
            if (isTraining) {
                isTraining = false;
                statusMessage = "Training stopped by user";
                statusColor = YELLOW;
            } else {
                showTrainingProgress = false;
                if (currentModel != nullptr) {
                    delete currentModel;
                    currentModel = nullptr;
                }
            }
        }
    }
}
