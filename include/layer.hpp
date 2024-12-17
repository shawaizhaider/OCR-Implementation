#ifndef LAYER_HPP
#define LAYER_HPP

#include <iostream>
#include <math.h>
#include <vector>
#include "neuron.hpp"
using namespace std;

class Layer{
    private:
        int layerSize;
        vector<Neuron*> neurons;
    public:
        Layer(int size);
        ~Layer();    
};

#endif // LAYER_HPP