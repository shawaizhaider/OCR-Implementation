#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <iostream>
#include <vector>
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
        vector<Layer*> layers;
        vector<double> input;
    public: 
        neuralNetwork(vector<int> topology);
        void setCurrentInput(vector<double> inputVals);
        void printNetwork();
        void feedForward();

        Matrix* getNeuronMatrix(int index){return this->layers[index]->convertValsToMatrix();}
        Matrix* getActivationMatrix(int index){return this->layers[index]->convertActivationValsToMatrix();}
        Matrix* getDifferentiatedMatrix(int index){return this->layers[index]->convertDifferentiatedValsToMatrix();}
        Matrix* getWeightMatrix(int index){return this->weightsMatrices[index];}

        void setNeuronVal(int layerIndex, int neuronIndex, double val){this->layers[layerIndex]->setVal(neuronIndex, val);}
        ~neuralNetwork();
};


#endif // NEURALNETWORK_HPP