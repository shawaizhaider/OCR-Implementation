#include "../include/neuralNetwork.hpp"
#include "../include/utils/math.hpp"

neuralNetwork::neuralNetwork(vector<int> topology)
{
    this->topology = topology;
    this->numLayers = topology.size();
    for(int i = 0; i < this->numLayers; i++)
    {
        /*Since, Topology at a particular index tells about number of neurons in a layer*/
        Layer *layer = new Layer(topology[i]);
        this->layers.push_back(layer);
    }
    for(int i = 0; i < this->numLayers - 1; i++)
    {
        /*Number of rows in weight matrix = Number of neurons in current layer
        * Number of columns in weight matrix = Number of neurons in next layer
        */
        Matrix *weightMatrix = new Matrix(topology[i], topology[i+1], true);
        this->weightsMatrices.push_back(weightMatrix);
    }
}

void neuralNetwork::setCurrentInput(vector<double> inputVals)
{
    this->inputVals = inputVals;
    for(int i = 0; i < this->inputVals.size(); i++)
    {
        this->layers[0]->setVal(i, this->inputVals[i]);
    }
}

void neuralNetwork::printNetwork()
{
    for(int i = 0; i < this->numLayers; i++)
    {
        cout << "Layer " << i << endl;
        cout << "Neuron Values" << endl;
        this->layers[i]->convertValsToMatrix()->printMatrix();
        cout << "Neuron Activation Values" << endl;
        this->layers[i]->convertActivationValsToMatrix()->printMatrix();
        cout << "Neuron Differentiated Values" << endl;
        this->layers[i]->convertDifferentiatedValsToMatrix()->printMatrix();
        if(i<this->numLayers-1)
        {
            cout<<"\t\t**************************************"<<endl;
            cout<<"Weights Matrix between Layer "<<i<<" and Layer "<<i+1<<endl;
            weightsMatrices[i]->printMatrix();
            cout<<"\t\t**************************************"<<endl;
        }
    }
}

void neuralNetwork::setErrors(){
    // TODO: Implement a better cost function
    if(this->targets.size() == 0){
        cerr<<"Error: No Targets found for the neural network!" << endl;
        assert(false);
    }
    if(this->targets.size() != this->layers[this->numLayers - 1]->getNeurons().size()){
        cerr<<"Error: Number of targets and number of neurons in output layer do not match!" << endl;
        assert(false);
    }
    this->globalError = 0.0;
    int outputLayerIdx = this->numLayers - 1;
    vector<Neuron*> outputNeurons = this->layers[outputLayerIdx]->getNeurons();
    this->errors.resize(outputNeurons.size()); // so that errors can store each neuron’s error
    for(int i = 0; i < outputNeurons.size(); i++){
        double diff = outputNeurons[i]->getNeuronActivation() - this->targets[i];
        this->errors[i] = diff;
        this->globalError += diff;   
    }
    iterationErrors.push_back(this->globalError);
}

void neuralNetwork::feedForward() {
    // TODO: Fix Memory Leaks and add biases
  Matrix *a;  // Matrix of neurons to the left
  Matrix *b;  // Matrix of weights to the right of layer
  Matrix *c;  // Matrix of neurons to the next layer

  for(int i = 0; i < (this->numLayers - 1); i++) {
    a = this->getNeuronMatrix(i);
    b = this->getWeightMatrix(i);
    c = new Matrix(
          a->getNumRows(),
          b->getNumCols(),
          false
        );

    if(i != 0) {
      a = this->getActivationMatrix(i);
    }

    utils::Math::multiplyMatrix(a, b, c);

    for(int c_index = 0; c_index < c->getNumCols(); c_index++) {
      this->setNeuronVal(i + 1, c_index, c->getMatrixVal(0, c_index) /* + this->bias*/);
    }
  }
}

neuralNetwork::~neuralNetwork()
{
    //TODO: Implement destructor
}