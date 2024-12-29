#include "../include/neuralNetwork.hpp"
using namespace NN;

void neuralNetwork::loadWeights(string filename){
    ifstream file(filename);
    json j;
    file >> j;
    vector<vector<vector<double>>> weightSet = j["weights"];
    for(int i = 0; i < weightsMatrices.size(); i++){
        for(int j = 0; j < weightsMatrices[i]->getNumRows(); j++){
            for(int k = 0; k < weightsMatrices[i]->getNumCols(); k++){
                this->weightsMatrices[i]->setMatrixVal(j, k, weightSet[i][j][k]);
            }
        }
    }
    file.close();
}

void neuralNetwork::loadBiases(string filename){
    ifstream file(filename);
    json j;
    file >> j;
    vector<vector<double>> biasSet = j["biases"];
    this->biases.clear(); 
    for (int i = 0; i < biasSet.size(); i++) {
        for (int j = 0; j < biasSet[i].size(); j++) {
            this->biases.push_back(biasSet[i][j]);
        }
    }
    file.close();
}

/*To load weights and biases of model
*Example Format of file:
{
    "biases": [
        [
            19.98416493027319,
            20.104249272905488
        ],
        [
            33.12209965766054,
            -0.41979997628320137
        ]
    ],
    "weights": [
        [
            [
                15.88085533605992,
                16.366458942691235
            ],
            [
                13.457958624615069,
                13.760431435807476
            ],
            [
                8.964759856363562,
                9.041989960871078
            ]
        ],
        [
            [
                30.87683985340874,
                0.33673170324798457
            ],
            [
                31.35349950028851,
                0.09297715278026639
            ]
        ]
    ]
}
*/
void neuralNetwork::loadModel(string filename){
    ifstream file(filename);
    json j;
    file >> j;
    vector<vector<vector<double>>> weightSet = j["weights"];
    for(int i = 0; i < weightsMatrices.size(); i++){
        for(int j = 0; j < weightsMatrices[i]->getNumRows(); j++){
            for(int k = 0; k < weightsMatrices[i]->getNumCols(); k++){
                this->weightsMatrices[i]->setMatrixVal(j, k, weightSet[i][j][k]);
            }
        }
    }
    vector<vector<double>> biasSet = j["biases"];
    this->biases.clear(); 
    for (int i = 0; i < biasSet.size(); i++) {
        for (int j = 0; j < biasSet[i].size(); j++) {
            this->biases.push_back(biasSet[i][j]);
        }
    }
    file.close();
}