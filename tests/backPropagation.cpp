#include "../include/neuralNetwork.hpp"
#include "../include/matrix.hpp"
#include <vector>

#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{
    // #ifdef _WIN32
    //     system("cls");
    // #else
    // system("clear");
    // #endif
    /* 
    vector<int> topology = {3, 2, 1};
    vector<double> inputVals = {1.0, 0.0,1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);

    nn->feedForward();
    nn->setErrors();
    nn->printNetwork();
    cout<<"Total Error before back propagation: "<<nn->getTotalError()<<endl;
    nn->backPropagate();
    nn->feedForward();
    nn->setErrors();
    cout<<"After Back Propagation"<<endl;
    nn->printNetwork();
    cout<<"Total Error after back propagation: "<<nn->getTotalError()<<endl;
    */

    // auto-encoder test
    vector<int> topology = {3, 2, 3};
    vector<double> inputVals = {1.0, 0.0, 1.0};
    neuralNetwork *nn = new neuralNetwork(topology);
    nn->setCurrentInput(inputVals);
    nn->setTargets(inputVals);

    // Training the network
    for(int i = 0; i < 25; i++){
    cout<<"Epoch: "<<i+1<<endl;
    nn->feedForward();
    nn->setErrors();
    cout<<"Total Error: "<<nn->getTotalError()<<endl;
    nn->backPropagate();
    }
    cout<<"*************************************"<<endl;
    /*Predicted Output*/
    nn->printOutput();
    cout<<"*************************************"<<endl;
    /*Target Output*/
    nn->printTargets();
    /*Iteration Errors*/
    nn->printiterationErrors();
    cout<<"*************************************"<<endl;
    nn->printNetwork();


    return 0;
}