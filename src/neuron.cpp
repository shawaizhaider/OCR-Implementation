#include "../include/neuron.hpp"
#include <math.h>

Neuron::Neuron(double val)
{
    this->neuronVal = val;
    activate();
    differentiate();
}

Neuron::Neuron(double val, int activationFunction)
{
    this->neuronVal = val;
    this->activationFunction = activationFunction;
    activate();
    differentiate();
}


void Neuron::initNeuron(double val)
{
    this->neuronVal = val;
    activate();
    differentiate(); 
}
/*Might need to change the behaviour of initNeuron */
void Neuron::initNeuron(double val, int activationFunction)
{
    this->neuronVal = val;
    this->activationFunction = activationFunction;
    activate();
    differentiate();
}

void Neuron::setVal(double val)
{
    this->neuronVal = val;
    activate();
    differentiate();
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

void Neuron::sigmoid()
{
    this->neuronActivation = 1/(1+exp(-this->neuronVal));
}

void Neuron::sigmoidDifferentiate(){
    this->differentiatedVal = this->neuronActivation*(1-this->neuronActivation);
}

void Neuron::Tanh()
{
    this->neuronActivation = tanh(this->neuronVal);
}

void Neuron::TanhDifferentiate()
{
    this->differentiatedVal = 1.0 - pow(this->neuronActivation, 2);
}

void Neuron::ReLU()
{
    this->neuronActivation = max(0.0, this->neuronVal);
}

void Neuron::ReLUDifferentiate()
{
    this->differentiatedVal = (this->neuronVal > 0) ? 1 : 0;
}
void Neuron::activate()
{
    switch(this->activationFunction)
    {
        case FASTSIGMOID:
            fastSigmoid();
            break;
        case SIGMOID:
            sigmoid();
            break;
        case RELU:
            ReLU();
            break;
        case TANH:
            Tanh();
            break;
        default:
            cerr<<"Error: Activation Function not available"<<endl;
            break;
    }
}

void Neuron::differentiate()
{
    switch(this->activationFunction)
    {
        case FASTSIGMOID:
            fastSigmoidDifferentiate();
            break;
        case SIGMOID:
            sigmoidDifferentiate();
            break;
        case RELU:
            ReLUDifferentiate();
            break;
        case TANH:
            TanhDifferentiate();
            break;
        default:
            cerr<<"Error: Activation Function not available"<<endl;
            break;
    }
}

Neuron::~Neuron() {
    // TODO: Implement destructor
    // delete this;
}
