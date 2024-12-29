#include "../include/neuralNetwork.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <ostream>
#include <streambuf>
#include <time.h>

using namespace std;
using json = nlohmann::json;
using namespace NN;

void printSyntax(){
    cout<<"Correct Syntax: " << endl;
    cout<<"Train [configFile]" << endl;
}


int main(int argc, char** argv){
    

    if(argc != 2){
        printSyntax();
        return 1;
    }

    ifstream trainingConfig(argv[1]);
    string str = string((std::istreambuf_iterator<char>(trainingConfig)), std::istreambuf_iterator<char>());
    auto config = json::parse(str);


    double learningRate = config["learningRate"];
    double momentum = config["momentum"];
    int maxEpochs = config["maxEpochs"];
    string trainingFile = config["trainingSet"];
    string labelsFile = config["labelsFile"];
    string weightsFile = config["weightsFile"];
    cout<<"Learning Rate: " << learningRate << endl;
    cout<<"Momentum: " << momentum << endl;
    cout<<"Max Epochs: " << maxEpochs << endl;
    cout<<"Training File: " << trainingFile << endl;
    cout<<"Labels File: " << labelsFile << endl;
    cout<<"Weights File: " << weightsFile << endl;


    vector<int> topology = config["topology"];
    neuralNetwork *nn = new neuralNetwork(topology);

    vector< vector<double>> trainingData = readData::readCSV::fetchData(trainingFile);
    vector< vector<double>> labels = readData::readCSV::fetchData(labelsFile);

    cout << "Training samples: " << trainingData.size() 
         << ", Features per sample: " 
         << (trainingData.empty() ? 0 : trainingData[0].size()) << endl;
    cout << "Label samples: " << labels.size() 
         << ", Values per label: " 
         << (labels.empty() ? 0 : labels[0].size()) << endl;

    // Basic checks for row sizes
    for (int i = 0; i < trainingData.size(); i++) {
        if (trainingData[i].size() != 3) {
            cerr << "Error: trainingData row " << i << " has size "
                 << trainingData[i].size() << " instead of 3." << endl;
            return 1;
        }
        if (labels[i].size() != 2) {
            cerr << "Error: labels row " << i << " has size "
                 << labels[i].size() << " instead of 2." << endl;
            return 1;
        }
    }

    if(trainingData.size() != labels.size()) {
        cerr << "Mismatch in rows of trainingData and labels." << endl;
        return 1;
    }

    for(int i = 0; i < maxEpochs; i++){
        for(int j = 0; j < trainingData.size(); j++){
            vector<double> input = trainingData[j];
            vector<double> target = labels[j];
            nn->setCurrentInput(input);
            nn->setTargets(target);
            nn->feedForward();
            nn->setErrors();
            nn->backPropagate();
        }
        cout<<"Epoch: " << i+1 << endl;
        cout<<"Error at epoch " << i + 1<< ": "<<nn->getTotalError()<<endl;
    }
    cout<<"Training Complete!" << endl;
    cout<<"Network Summary" << endl;
    nn->printNetwork();
    nn->printOutput();
    nn->printTargets();
    
    nn->saveModel(weightsFile);
    cout<<"Model Saved!" << endl;
    return 0;
}