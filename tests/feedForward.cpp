#include "../include/neuralNetwork.hpp"
#include "../include/matrix.hpp"
#include <vector>

#include <iostream>
#include <cstdlib>
using namespace std;
using namespace NN;

int main()
{
    #ifdef _WIN32
        system("cls");
    #else
    system("clear");
    #endif
    /* 
    vector<int> topology = {3, 2, 1};
    vector<double> inputVals = {1.0, 0.0,1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->feedForward();
    nn->printNetwork();
    */

    // auto-encoder test
    vector<int> topology = {3, 2, 3};
    vector<double> inputVals = {1.0, 0.0, 1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->setTargets(inputVals);
    nn->feedForward();
    nn->setErrors();
    nn->printNetwork();

    cout<<"Total Error: "<<nn->getTotalError()<<endl;
    return 0;
}