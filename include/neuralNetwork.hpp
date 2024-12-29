#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "matrix.hpp"
#include "layer.hpp"
#include "json.hpp"
#include "utils/math.hpp"
#include "utils/read_csv.hpp"

using namespace std;
using json = nlohmann::json;

enum ActivationFunction{
    FASTSGMD,
    SIGMD,
    relu,
    tanH
};

struct NNConfig{
    vector<int> topology;
    double learningRate;
    double momentum;
    int maxEpochs;
    ActivationFunction hiddenActivationFunction;
    ActivationFunction outputActivationFunction;
    string trainingFile;
    string labelsFile;
    string weightsFile;
};
class neuralNetwork{
    private:
        /*Topology is an array of values that tells the number of neurons in each layer of
        *the neural network. For example, if the topology is {2, 3, 1}, then the neural network
        *has 2 neurons in the input layer, 3 neurons in the hidden layer, and 1 neuron in the output.
        * Number of Weight Matrices = Number of Layers - 1
        * or Number of Weight Matrices = Number of elements in Topology - 1
        * Number of Topologies = Number of elements in Topology
        * Number of Topologies = Number of Layers
        * Topology Index = Layer Index
        */
        vector<int> topology;
        int numLayers;
        vector<double> inputVals;
        vector<NN::Matrix*> weightsMatrices;
        vector<NN::Matrix*> gradients;
        vector<Layer*> layers;
        vector<double> input;
        vector<double> output;
        /*Errors of model or differences*/
        double globalError;   // Total Error
        vector<double> errors;  // Errors of each target and output pair
        vector<double> differentiatedErrors; // Differentiated Errors
        vector<double> iterationErrors; // Total Error in each iteration
        vector<double> targets;
        double learning_rate = 1;
        double momentum = 1;
        vector<double> biases;
        vector<vector<double>> biasesVector;
        NNConfig config;
        ActivationFunction hiddenActivationFunction;
        ActivationFunction outputActivationFunction;
        int epochs;
    public: 
        neuralNetwork(NNConfig config);
        neuralNetwork(vector<int> topology);
        void train(vector<double> inputVals, vector<double> targets, double bias, double learningRate, double momentum);
        void setCurrentInput(vector<double> inputVals);
        void printNetwork();
        void feedForward();
        void backPropagate();
        NN::Matrix* getNeuronMatrix(int index){return this->layers[index]->convertValsToMatrix();}
        NN::Matrix* getActivationMatrix(int index){return this->layers[index]->convertActivationValsToMatrix();}
        NN::Matrix* getDifferentiatedMatrix(int index){return this->layers[index]->convertDifferentiatedValsToMatrix();}
        NN::Matrix* getWeightMatrix(int index){return this->weightsMatrices[index];}
        int getBiasIndex(int layerIndex, int neuronIndex);
        vector<double> getBiases(){return this->biases;}
        void setNeuronVal(int layerIndex, int neuronIndex, double val){this->layers[layerIndex]->setVal(neuronIndex, val);}
        double getTotalError(){return this->globalError;};
        vector<double> getErrors(){return this->errors;};
        void setTargets(vector<double> targets){this->targets = targets;}
        void train();
        void train(vector<double> input, vector<double> targets);
        void setErrors();
        void printInput();
        void printOutput();
        void printTargets();
        void printiterationErrors();
        void saveWeights(string file);
        void saveBiases(string file);
        void loadWeights(string file);
        void loadBiases(string file);
        void saveModel(string file);
        void loadModel(string file);
        void setBias(int index, double val){ this->biases[index] = val; }
        double getBias(int index){ return this->biases[index]; }
        double getGlobalError(){ return this->globalError; } 
        vector<double> getOutput();
        ~neuralNetwork();
};


#endif // NEURALNETWORK_HPP