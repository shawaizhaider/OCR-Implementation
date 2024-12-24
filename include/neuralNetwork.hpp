#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include "matrix.hpp"
#include "layer.hpp"

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
        vector<Matrix*> weightsMatrices;
        vector<Matrix*> gradients;
        vector<Layer*> layers;
        vector<double> input;
        /*Errors of model or differences*/
        double globalError;   // Total Error
        vector<double> errors;  // Errors of each target and output pair
        vector<double> iterationErrors; // Total Error in each iteration
        vector<double> targets;
    public: 
        neuralNetwork(vector<int> topology);
        void setCurrentInput(vector<double> inputVals);
        void printNetwork();
        void feedForward();
        void backPropagate();
        Matrix* getNeuronMatrix(int index){return this->layers[index]->convertValsToMatrix();}
        Matrix* getActivationMatrix(int index){return this->layers[index]->convertActivationValsToMatrix();}
        Matrix* getDifferentiatedMatrix(int index){return this->layers[index]->convertDifferentiatedValsToMatrix();}
        Matrix* getWeightMatrix(int index){return this->weightsMatrices[index];}
        void setNeuronVal(int layerIndex, int neuronIndex, double val){this->layers[layerIndex]->setVal(neuronIndex, val);}
        double getTotalError(){return this->globalError;};
        vector<double> getErrors(){return this->errors;};
        void setTargets(vector<double> targets){this->targets = targets;}
        void setErrors();
        void printInput();
        void printOutput();
        void printTargets();
        void printiterationErrors();
        ~neuralNetwork();
};


#endif // NEURALNETWORK_HPP