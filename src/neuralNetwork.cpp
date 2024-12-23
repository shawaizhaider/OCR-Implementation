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

void neuralNetwork::backPropagate(){
    vector<Matrix* > updatedWeights;
    Matrix* gradient;
    int outputLayerIdx = this->numLayers - 1;
    /*OtoH -> Output to Hidden*/
    Matrix* differentiatedVals_OtoH = this->layers[outputLayerIdx]->convertDifferentiatedValsToMatrix();
    Matrix* gradients_OtoH = new Matrix(1, this->layers[outputLayerIdx]->getNeurons().size(), false);
    for(int i=0; i<this->errors.size(); i++){
        double differentiatedVal = differentiatedVals_OtoH->getMatrixVal(0, i);
        double errorVal = this->errors[i];
        double gradientVal = differentiatedVal * errorVal;
        gradients_OtoH->setMatrixVal(0, i, gradientVal);
    }
    /*Index Hidden Layer on the left of Output Layer*/
    int hiddenLayerIdx = outputLayerIdx - 1;
    /*Hidden Layer on the left of Output Layer*/
    Layer* hiddenLayer = this->layers[hiddenLayerIdx];
    Matrix* weightsOtoH = this->weightsMatrices[outputLayerIdx - 1];
    /*Might need a fix here*/
    Matrix* deltaOtoH = utils::Math::multiplyTwoMatrix(gradients_OtoH->transpose(), hiddenLayer->convertActivationValsToMatrix())->transpose();
    Matrix* updatedWeightsOtoH = new Matrix(deltaOtoH->getNumRows(), deltaOtoH->getNumCols(), false);
    for(int j=0; j<deltaOtoH->getNumRows(); j++){
        for(int k=0; k<deltaOtoH->getNumCols(); k++){
            double deltaWeightVal = deltaOtoH->getMatrixVal(j, k);
            double weightVal = weightsOtoH->getMatrixVal(j, k);
            updatedWeightsOtoH->setMatrixVal(j, k, weightVal - deltaWeightVal);
        }
    }
    updatedWeights.push_back(updatedWeightsOtoH);
    /*Make a copy of gradients_OtoH in gradient*/
    gradient = new Matrix(gradients_OtoH->getNumRows(), gradients_OtoH->getNumCols(), false);
    for(int i=0; i<gradients_OtoH->getNumRows(); i++){
        for(int j=0; j<gradients_OtoH->getNumCols(); j++){
            gradient->setMatrixVal(i, j, gradients_OtoH->getMatrixVal(i, j));
        }
    }
    /*For Debugging
    cout<<"Updated Weights between Hidden Layer and Output Layer"<<endl;
    updatedWeightsOtoH->printMatrix();
    */
    // Hidden Layer to Input layer
    for(int i=outputLayerIdx - 1; i > 0; i--){
        Layer* currentLayer = this->layers[i];
        /*Differentiated Values of the current layer, i.e., hidden layer.*/
        Matrix* differentiatedVals = currentLayer->convertDifferentiatedValsToMatrix();
        /*Activated Values of the current layer, i.e., hidden layer.*/
        Matrix* activatedVals = currentLayer->convertActivationValsToMatrix();
        /*Gradients of the current layer, i.e., hidden layer.*/
        Matrix* differentiaredGradients = new Matrix(1, currentLayer->getNeurons().size(), false);
        /*Weight matrix to the right of current hidden layer*/
        Matrix* weightMatrix = this->weightsMatrices[i];
        /*Weight matrix to the left of current hidden layer*/
        Matrix* weightMatrixLeft = this->weightsMatrices[i - 1];

        for(int i = 0; i < weightMatrix->getNumRows(); i++){
            /*Sum of Products of gradient and Weight Matrix*/
            double SOP = 0;
            for(int j = 0; j < weightMatrix->getNumCols(); j++){
                double product = gradient->getMatrixVal(0,j)*weightMatrix->getMatrixVal(i,j);
                SOP += product;
            }
            double gradientVal = SOP * activatedVals->getMatrixVal(0,i);
            differentiaredGradients->setMatrixVal(0, i, gradientVal);
        }
        Matrix* leftLayer = (i-1) == 0 ? this->layers[0]->convertValsToMatrix() : this->layers[i - 1]->convertActivationValsToMatrix();
        /*Might need a fix here*/
        Matrix* delta = utils::Math::multiplyTwoMatrix(differentiaredGradients->transpose(), leftLayer)->transpose();
        Matrix* updatedWeightsHidden = new Matrix(delta->getNumRows(), delta->getNumCols(), false);
        for(int i = 0; i < updatedWeightsHidden->getNumRows(); i++){
            for(int j = 0; j < updatedWeightsHidden->getNumCols(); j++){
                double deltaWeightVal = delta->getMatrixVal(i, j);
                double origWeightVal = weightMatrixLeft->getMatrixVal(i, j);
                updatedWeightsHidden->setMatrixVal(i, j, origWeightVal - deltaWeightVal);
            }
        }
        updatedWeights.push_back(updatedWeightsHidden);
        /*Make a copy of differentiaredGradients in gradient*/
        gradient = new Matrix(differentiaredGradients->getNumRows(), differentiaredGradients->getNumCols(), false);
        for(int i=0; i<differentiaredGradients->getNumRows(); i++){
            for(int j=0; j<differentiaredGradients->getNumCols(); j++){
                gradient->setMatrixVal(i, j, differentiaredGradients->getMatrixVal(i, j));
            }
        }
    }
    /*For Debugging
    cout<<"Completed Back Propagation"<<endl;
    cout << "Updated weights size: " << updatedWeights.size() << endl; 
    cout << "Old Weights size: " << this->weightsMatrices.size() << endl;
    */
    reverse(updatedWeights.begin(), updatedWeights.end());
    this->weightsMatrices = updatedWeights;
}

neuralNetwork::~neuralNetwork()
{
    //TODO: Implement destructor
}