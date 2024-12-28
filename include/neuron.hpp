#ifndef _NEURON_HPP
#define _NEURON_HPP

#define FASTSIGMOID 0
#define SIGMOID 1
#define RELU 2
#define TANH 3

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
        /*Selected Fast Sigmoid by default*/
        int activationFunction = FASTSIGMOID;
    public:
        Neuron(double val);
        Neuron(double val, int activationFunction);
        void initNeuron(double val);
        void initNeuron(double val, int activationFunction);
        void setVal(double val);
        // f(x) = x/1+|x| 
        void fastSigmoid();
        // f'(x) = f(x)(1-f(x))
        void fastSigmoidDifferentiate();
        void sigmoid();
        void sigmoidDifferentiate();
        void ReLU();
        void ReLUDifferentiate();
        void Tanh();
        void TanhDifferentiate();
        void activate();
        void differentiate();
        // Getters
        double getNeuronVal(){return this->neuronVal;}
        double getNeuronActivation(){return this->neuronActivation;}
        double getDifferentiatedVal(){return this->differentiatedVal;}
        ~Neuron();

};


#endif // _NEURON_HPP