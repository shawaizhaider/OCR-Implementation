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
        void setVal(int index, double val);
        vector<Neuron*> getNeurons(){return this->neurons;}
        void setNeurons(vector<Neuron*> neurons){this->neurons = neurons;}
        Matrix* convertValsToMatrix();
        Matrix* convertActivationValsToMatrix();
        Matrix* convertDifferentiatedValsToMatrix();
        ~Layer();    
};

#endif // LAYER_HPP