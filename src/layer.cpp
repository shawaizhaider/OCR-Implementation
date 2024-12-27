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

Layer::Layer(int size, int activationFunction)
{
    this->layerSize = size;
    for(int i = 0; i < size; i++)
    {
        Neuron *neuron = new Neuron(0.00, activationFunction);
        this->neurons.push_back(neuron);
    }
}

void Layer::setVal(int index, double val)
{
    this->neurons[index]->initNeuron(val);
}

/*TODO: Fix memory leaks*/
Matrix* Layer::convertValsToMatrix()
{
    Matrix *matrix = new Matrix(1, this->layerSize, false);
    for(int i = 0; i < this->layerSize; i++)
    {
        matrix->setMatrixVal(0, i, this->neurons[i]->getNeuronVal());
    }
    return matrix;
}
Matrix* Layer::convertActivationValsToMatrix()
{
    Matrix *matrix = new Matrix(1, this->layerSize, false);
    for(int i = 0; i < this->layerSize; i++)
    {
        matrix->setMatrixVal(0, i, this->neurons[i]->getNeuronActivation());
    }
    return matrix;
}
Matrix* Layer::convertDifferentiatedValsToMatrix()
{
    Matrix *matrix = new Matrix(1, this->layerSize, false);
    for(int i = 0; i < this->layerSize; i++)
    {
        matrix->setMatrixVal(0, i, this->neurons[i]->getDifferentiatedVal());
    }
    return matrix;
}
Layer::~Layer()
{
    for(int i = 0; i < this->layerSize; i++)
    {
        delete this->neurons[i];
    }
}