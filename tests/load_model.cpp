#include "../include/neuralNetwork.hpp"
#include "../include/json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <ostream>
#include <streambuf>
#include <time.h>

using namespace std;
using namespace NN;
using json = nlohmann::json;

void printSyntax(){
    cout<<"Correct Syntax: " << endl;
    cout<<"predict [Evaluation Config-File]" << endl;
}


int main(int argc, char** argv){
    

    if(argc != 2){
        printSyntax();
        return 1;
    }

    ifstream testConfig(argv[1]);
    string str = string((std::istreambuf_iterator<char>(testConfig)), std::istreambuf_iterator<char>());
    auto config = json::parse(str);


    string testFile = config["testFile"];
    string weightsFile = config["weightsFile"];
    string labelsFile = config["labelsFile"];
    cout<<"Weights File: " << weightsFile << endl;

    vector<int> topology = config["topology"];
    neuralNetwork *nn = new neuralNetwork(topology);

    vector< vector<double>> tests = readData::readCSV::fetchData(testFile);
    vector< vector<double>> labels = readData::readCSV::fetchData(labelsFile);

    cout << "Test samples: " << tests.size() 
         << ", Features per sample: " 
         << (tests.empty() ? 0 : tests[0].size()) << endl; 

    nn->loadModel(weightsFile);
    cout << "Model loaded successfully!" << endl;

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

    return 0;
}