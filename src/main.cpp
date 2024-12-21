#include <iostream>
#include <cstdlib>
#include "../include/neuron.hpp"
#include "../include/matrix.hpp"
#include "../include/neuralNetwork.hpp"

using namespace std;

int main(int argc, char const **argv)
{
    system("clear");
    /*Testing Neuron Class*/
    // Neuron *neuron1;
    // neuron1 = new Neuron(1.5);
    // cout << "Neuron Value: " << neuron1->getNeuronVal() << endl;
    // cout << "Neuron Activation: " << neuron1->getNeuronActivation() << endl;
    // cout << "Neuron Differentiated Value: " << neuron1->getDifferentiatedVal() << endl;
    
    // delete neuron1;

    /*Testing Matrix Class*/
    // Matrix *matrix = new Matrix(3,2,true);
    // matrix->printMatrix();

    // cout << "Transposed Matrix" << endl;
    // Matrix *transposedMatrix = matrix->transpose();
    // transposedMatrix->printMatrix();

    // delete matrix;
    // delete transposedMatrix;


    /*Testing Neiral Network Class */
    vector<int> topology = {3, 2, 3};
    vector<double> inputVals = {1.0, 0.0, 1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->printNetwork();
    delete nn;
    
    return 0;
}
