#include "../include/neuralNetwork.hpp"

using namespace NN;

void neuralNetwork::saveWeights(string filename){
    json j = {};
    vector<vector<vector<double>>> weightSet;
    for(int i = 0; i < this->weightsMatrices.size(); i++){
        weightSet.push_back(this->weightsMatrices[i]->getMatrixVals());
    }
    j["weights"] = weightSet;  
    ofstream file(filename);
    if(file.is_open()){
        file << j.dump(4);
        file.close();
        cout << "Weights saved to " << filename << endl;
    } else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

void neuralNetwork::saveModel(string filename){
    json j = {};

    vector<vector<vector<double>>> weightSet;
    for (int i = 0; i < this->weightsMatrices.size(); i++) {
        weightSet.push_back(this->weightsMatrices[i]->getMatrixVals());
    }
    j["weights"] = weightSet;

    vector<vector<double>> biasSet;
    for (int i = 1; i < this->numLayers; i++) {  
        vector<double> biasesForLayer;
        for (int j = 0; j < this->layers[i]->getNeurons().size(); j++) {
            biasesForLayer.push_back(this->biases[this->getBiasIndex(i, j)]);
        }
        biasSet.push_back(biasesForLayer);
    }
    j["biases"] = biasSet;

    ofstream file(filename);
    if (file.is_open()) {
        file << j.dump(4); 
        file.close();
        cout << "Weights and biases saved to " << filename << endl;
    } else {
        cerr << "Failed to open file: " << filename << endl;
    }
}

