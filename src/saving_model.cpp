#include "../include/neuralNetwork.hpp"


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

void neuralNetwork::loadWeights(string filename){
    ifstream file(filename);
    json j;
    file >> j;
    vector<vector<vector<double>>> weightSet = j["weights"];
    for(int i = 0; i < weightSet.size(); i++){
        Matrix* weightMatrix = new Matrix(weightSet[i].size(), weightSet[i][0].size(), false);
        for(int j = 0; j < weightSet[i].size(); j++){
            for(int k = 0; k < weightSet[i][j].size(); k++){
                weightMatrix->setMatrixVal(j, k, weightSet[i][j][k]);
            }
        }
        this->weightsMatrices.push_back(weightMatrix);
    }
    file.close();
}

void neuralNetwork::loadBiases(string filename){
    ifstream file(filename);
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        double val;
        while(ss >> val){
            this->biases.push_back(val);
        }
    }
    file.close();
}

void neuralNetwork::loadModel(string filename){
    ifstream file(filename);
    string line;
    int i = 0;
    while(getline(file, line)){
        if(i == 0){
            stringstream ss(line);
            int numLayers;
            ss >> numLayers;
            this->numLayers = numLayers;
        }
        else if(i == 1){
            stringstream ss(line);
            int val;
            while(ss >> val){
                this->topology.push_back(val);
            }
        }
        else if(i > 1 && i <= this->numLayers + 1){
            vector<double> row;
            stringstream ss(line);
            double val;
            while(ss >> val){
                row.push_back(val);
            }
            Matrix* weightMatrix = new Matrix(row.size(), 1, false);
            for(int j = 0; j < row.size(); j++){
                weightMatrix->setMatrixVal(j, 0, row[j]);
            }
            this->weightsMatrices.push_back(weightMatrix);
        }
        else{
            stringstream ss(line);
            double val;
            while(ss >> val){
                this->biases.push_back(val);
            }
        }
        i++;
    }
    file.close();
}
