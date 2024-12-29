#include <iostream>
#include <cstdlib>
#include "../include/neuralNetwork.hpp"

using namespace std;
using namespace NN;

int main(int argc, char const **argv)
{
    #ifdef _WIN32
        system("cls");
    #else
    system("clear");
    #endif

    /*Testing Neural Network Class */
    vector<int> topology = {3, 2, 3};
    vector<double> inputVals = {1.0, 0.0, 1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->printNetwork();
    delete nn;
    
    return 0;
}
