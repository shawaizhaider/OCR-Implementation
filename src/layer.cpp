#include "../include/layer.hpp"

Layer::Layer(int size)
{
    this->layerSize = size;
    for(int i = 0; i < size; i++)
    {
        Neuron *neuron = new Neuron(0.00);
        this->neurons.push_back(neuron);
    }
}
Layer::~Layer()
{
    //TODO: Implement destructor
    for(int i = 0; i < this->layerSize; i++)
    {
        delete this->neurons[i];
    }
}