#include "../include/neuralNetwork.hpp"
#include "../include/matrix.hpp"
#include <vector>

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    #ifdef _WIN32
        system("cls");
    #else
    system("clear");
    #endif
    vector<int> topology = {3, 2, 1};
    vector<double> inputVals = {1.0, 0.0,1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->feedForward();
    nn->printNetwork();
    return 0;
}