#ifndef _NEURON_HPP
#define _NEURON_HPP

#include <iostream>
using namespace std;

class Neuron
{
    private:
        double neuronVal;
        //after passing through activation function
        double neuronActivation;
        //value after taking derivative of activation function
        double differentiatedVal;
    public:
        Neuron(double val);
        void initNeuron(double val);
        // f(x) = x/1+|x| 
        void fastSigmoid();
        // f'(x) = f(x)(1-f(x))
        void fastSigmoidDifferentiate();
        // Getters
        double getNeuronVal(){return this->neuronVal;}
        double getNeuronActivation(){return this->neuronActivation;}
        double getDifferentiatedVal(){return this->differentiatedVal;}
        ~Neuron();

};


#endif // _NEURON_HPP