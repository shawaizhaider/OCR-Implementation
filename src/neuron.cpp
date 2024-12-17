#include "../include/neuron.hpp"
#include <math.h>

Neuron::Neuron(double val)
{
    this->neuronVal = val;
    fastSigmoid();
    fastSigmoidDifferentiate();
}


void Neuron::initNeuron(double val)
{
    this->neuronVal = val;
    fastSigmoid();
    fastSigmoidDifferentiate();
}
// f(x) = x/1+|x| 
void Neuron::fastSigmoid()
{
    this->neuronActivation = this->neuronVal/(1+abs(this->neuronVal));
}
// f'(x) = f(x)(1-f(x))
void Neuron::fastSigmoidDifferentiate()
{
    this->differentiatedVal = this->neuronActivation*(1-this->neuronActivation);
}

Neuron::~Neuron() {
    // TODO: Implement destructor
    // delete this;
}
