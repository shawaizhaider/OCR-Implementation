#include <iostream>
#include <cstdlib>
#include "../include/neuron.hpp"

using namespace std;

int main(int argc, char const **argv)
{
    #ifdef _WIN32
        system("cls");
    #else
    system("clear");
    #endif
    /*Testing Neuron Class*/
    Neuron *neuron1;
    neuron1 = new Neuron(1.5);
    cout << "Neuron Value: " << neuron1->getNeuronVal() << endl;
    cout << "Neuron Activation: " << neuron1->getNeuronActivation() << endl;
    cout << "Neuron Differentiated Value: " << neuron1->getDifferentiatedVal() << endl;
    
    delete neuron1;

    return 0;
}
