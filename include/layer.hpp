#ifndef LAYER_HPP
#define LAYER_HPP

#include <iostream>
#include <math.h>
#include <vector>
#include "neuron.hpp"
#include "matrix.hpp"
using namespace std;
class Layer{
    private:
        int layerSize;
        vector<Neuron*> neurons;
    public:
        Layer(int size);
        Layer(int size, int activationFunction);
        void setVal(int index, double val);
        vector<double> getActivationVals();
        vector<Neuron*> getNeurons(){return this->neurons;}
        void setNeurons(vector<Neuron*> neurons){this->neurons = neurons;}
        NN::Matrix* convertValsToMatrix();
        NN::Matrix* convertActivationValsToMatrix();
        NN::Matrix* convertDifferentiatedValsToMatrix();
        ~Layer();    
};

#endif // LAYER_HPP