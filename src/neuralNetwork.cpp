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

    // Initializing biases for all neurons (except input layer since it doesn't have biases)
    for(int i = 1; i < this->numLayers; i++){
        for(int j = 0; j < this->topology[i]; j++){
            this->biases.push_back(0.0); 
        }
    }
}

neuralNetwork::neuralNetwork(NNConfig config)
{
    this->config = config;
    this->topology = config.topology;
    this->numLayers = topology.size();
    this->epochs = config.maxEpochs;
    this->learning_rate = config.learningRate;
    this->momentum = config.momentum;

    this -> hiddenActivationFunction = config.hiddenActivationFunction;
    this -> outputActivationFunction = config.outputActivationFunction;
    for(int i = 0; i < this->numLayers; i++)
    {
        if(i > 0 && i < this->numLayers - 1){
            Layer *layer = new Layer(topology[i], this->hiddenActivationFunction);
            this->layers.push_back(layer);
        } else if(i == this->numLayers - 1){
            Layer *layer = new Layer(topology[i], this->outputActivationFunction);
            this->layers.push_back(layer);
        }
        else{
            Layer *layer = new Layer(topology[i]);
            this->layers.push_back(layer);
        }
    }
    for(int i = 0; i < this->numLayers - 1; i++)
    {
        /*Number of rows in weight matrix = Number of neurons in current layer
        * Number of columns in weight matrix = Number of neurons in next layer
        */
        Matrix *weightMatrix = new Matrix(topology[i], topology[i+1], true);
        this->weightsMatrices.push_back(weightMatrix);
    }

    for(int i = 0; i < topology[topology.size() - 1]; i++){
        errors.push_back(0.0);
        differentiatedErrors.push_back(0.0);
    }

    // Initializing biases for all neurons (except input layer since it doesn't have biases)
    for(int i = 1; i < this->numLayers; i++){
        for(int j = 0; j < this->topology[i]; j++){
            this->biases.push_back(0.0); 
        }
    }
    this -> globalError = 0.0;
}

void neuralNetwork::setCurrentInput(vector<double> inputVals)
{
    this->inputVals = inputVals;
    for(int i = 0; i < this->inputVals.size(); i++)
    {
        this->layers[0]->setVal(i, this->inputVals[i]);
    }
}

int neuralNetwork::getBiasIndex(int layerIndex, int neuronIndex){
    int offset = 0;
    for(int i=1; i<layerIndex; i++){
        offset += this->topology[i];
    }
    return offset + neuronIndex;
}

void neuralNetwork::printNetwork()
{
    for(int i = 0; i < this->numLayers; i++)
    {
        cout << "Layer " << i << endl;

        Matrix* valsMatrix = this->layers[i]->convertValsToMatrix();
        cout << "Neuron Values" << endl;
        valsMatrix->printMatrix();
        delete valsMatrix;

        Matrix* actMatrix = this->layers[i]->convertActivationValsToMatrix();
        cout << "Neuron Activation Values" << endl;
        actMatrix->printMatrix();
        delete actMatrix;

        Matrix* diffMatrix = this->layers[i]->convertDifferentiatedValsToMatrix();
        cout << "Neuron Differentiated Values" << endl;
        diffMatrix->printMatrix();
        delete diffMatrix;

        if(i<this->numLayers-1)
        {
            cout<<"\t\t**************************************"<<endl;
            cout<<"Weights Matrix between Layer "<<i<<" and Layer "<<i+1<<endl;
            this->weightsMatrices[i]->printMatrix();
            cout<<"\t\t**************************************"<<endl;
        }
    }

    // Printing biases layer-wise (skipping input layer since it doesn't have biases)
    cout << "Biases:" << endl;
    for(int i = 1; i < this->numLayers; i++){
        cout << "  Layer " << i << " biases: ";
        for(int neuronIndex = 0; neuronIndex < this->topology[i]; neuronIndex++){
            int biasIdx = getBiasIndex(i, neuronIndex);
            cout << this->biases[biasIdx] << " ";
        }
        cout << endl;
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

    /*Modified the implementation of cost function */

    // for(int i = 0; i < outputNeurons.size(); i++){
    //     double diff = outputNeurons[i]->getNeuronActivation() - this->targets[i];
    //     this->errors[i] = diff;
    //     differentiatedErrors.push_back(diff);
    //     // differentiatedErrors[i] = diff;
    //     this->globalError += pow(diff,2);   
    // }
    // this->globalError = this->globalError / 2;

    for(int i = 0; i < outputNeurons.size(); i++){
        double diff = outputNeurons[i]->getNeuronActivation() - this->targets[i];
        this->errors[i] = 0.5 * pow(abs(diff), 2);
        differentiatedErrors.push_back(diff);
        this->globalError += errors[i];   
    }
    iterationErrors.push_back(this->globalError);
}

void neuralNetwork::feedForward() {
    // TODO: Fix Memory Leaks and add biases
    /*Update: Added biases and fixed memory leaks to some extent*/
    for(int i = 0; i < (this->numLayers - 1); i++) {
        Matrix* a = (i == 0) ? this->getNeuronMatrix(i) : this->getActivationMatrix(i);
        Matrix* b = this->getWeightMatrix(i);
        Matrix* c = new Matrix(a->getNumRows(), b->getNumCols(), false);

        utils::Math::multiplyMatrix(a, b, c);
        for(int c_index = 0; c_index < c->getNumCols(); c_index++) {
            // Adding bias
            int biasIdx = getBiasIndex(i+1, c_index);
            double netVal = c->getMatrixVal(0, c_index) + this->biases[biasIdx];
            this->setNeuronVal(i + 1, c_index, netVal);
        }
        delete a;
        delete c;
    }
}

void neuralNetwork::backPropagate()
{
    // For Output Layer
    int outputLayerIdx = this->numLayers - 1;
    Matrix* derivedVals = this->layers[outputLayerIdx]->convertDifferentiatedValsToMatrix();
    Matrix* gradients = new Matrix(1, this->topology[outputLayerIdx], false);
    /* Computing gradients for output layer using differentiatedErrors */
    for(int i = 0; i < this->topology[outputLayerIdx]; i++){
        double e = this->differentiatedErrors[i];
        double dVal = derivedVals->getMatrixVal(0, i);
        gradients->setMatrixVal(0, i, e * dVal);
    }

    // deltaWeights = gradientsᵀ * activatedVals
    Matrix* gradT = gradients->transpose();
    Matrix* activatedVals = this->layers[outputLayerIdx - 1]->convertActivationValsToMatrix();
    Matrix* deltaWeights = new Matrix(gradT->getNumRows(), activatedVals->getNumCols(), false);
    utils::Math::multiplyMatrix(gradT, activatedVals, deltaWeights);

    // last hidden layer->output layer weights
    Matrix* newWeightMat = new Matrix(this->topology[outputLayerIdx - 1], this->topology[outputLayerIdx], false);
    for(int r = 0; r < newWeightMat->getNumRows(); r++){
        for(int c = 0; c < newWeightMat->getNumCols(); c++){
            double orig = this->weightsMatrices[outputLayerIdx - 1]->getMatrixVal(r, c);
            double dW = deltaWeights->getMatrixVal(c, r);
            orig = this->momentum * orig;
            dW = this->learning_rate * dW;
            newWeightMat->setMatrixVal(r, c, orig - dW);
        }
    }

    // Updating weights and biases for output layer
    for(int i = 0; i < this->topology[outputLayerIdx]; i++){
        double gradVal = gradients->getMatrixVal(0, i);
        int biasIdx = getBiasIndex(outputLayerIdx, i);
        this->biases[biasIdx] = (this->biases[biasIdx] * this->momentum) - (this->learning_rate * gradVal);
    }
    vector<Matrix*> updatedWeights;
    updatedWeights.push_back(new Matrix(*newWeightMat));

    // Memory cleanup
    delete derivedVals; 
    delete gradT;
    delete activatedVals;
    delete deltaWeights;
    delete newWeightMat;

    //Propagating backwards through hidden layers
    for(int layerIdx = outputLayerIdx - 1; layerIdx > 0; layerIdx--){
        // Copy old gradients
        Matrix* pGradients = new Matrix(*gradients);
        delete gradients;

        // transposed weights from next layer
        Matrix* nextLayerWT = this->weightsMatrices[layerIdx]->transpose();

        // gradients = pGradients * transposedWeights
        gradients = new Matrix(pGradients->getNumRows(), nextLayerWT->getNumCols(), false);
        utils::Math::multiplyMatrix(pGradients, nextLayerWT, gradients);

        // multiply by derivative
        Matrix* derivMatrix = this->layers[layerIdx]->convertDifferentiatedValsToMatrix();
        for(int col = 0; col < derivMatrix->getNumCols(); col++){
            double g = gradients->getMatrixVal(0, col) * derivMatrix->getMatrixVal(0, col);
            gradients->setMatrixVal(0, col, g);
        }

        Matrix* zMat = (layerIdx == 1) 
            ? this->layers[0]->convertValsToMatrix() 
            : this->layers[layerIdx - 1]->convertActivationValsToMatrix();
        Matrix* zT = zMat->transpose();
        Matrix* deltaW = new Matrix(zT->getNumRows(), gradients->getNumCols(), false);

        // deltaW = zT * gradients
        utils::Math::multiplyMatrix(zT, gradients, deltaW);

        Matrix* tempWeights = new Matrix(this->weightsMatrices[layerIdx - 1]->getNumRows(), 
                                         this->weightsMatrices[layerIdx - 1]->getNumCols(), false);
        for(int r = 0; r < tempWeights->getNumRows(); r++){
            for(int c = 0; c < tempWeights->getNumCols(); c++){
                double orig = this->weightsMatrices[layerIdx - 1]->getMatrixVal(r, c);
                double dW = deltaW->getMatrixVal(r, c);
                orig = this->momentum * orig;
                dW = this->learning_rate * dW;
                tempWeights->setMatrixVal(r, c, orig - dW);
            }
        }

        // Updating biases
        for(int neuron = 0; neuron < this->topology[layerIdx]; neuron++){
            double gradVal = gradients->getMatrixVal(0, neuron);
            int bIdx = getBiasIndex(layerIdx, neuron);
            this->biases[bIdx] = (this->biases[bIdx] * this->momentum) - (this->learning_rate * gradVal);
        }

        updatedWeights.push_back(new Matrix(*tempWeights));

        // Memory Cleanup
        delete pGradients;
        delete nextLayerWT;
        delete derivMatrix;
        delete zMat;
        delete zT;
        delete deltaW;
        delete tempWeights;
    }
    delete gradients;

    // Updating old weights
    for (auto oldW : this->weightsMatrices) delete oldW;
    this->weightsMatrices.clear();
    reverse(updatedWeights.begin(), updatedWeights.end());
    for (auto w : updatedWeights){
        this->weightsMatrices.push_back(new Matrix(*w));
        delete w;
    }
}

void neuralNetwork::printInput()
{
    cout << "Input Values" << endl;
    for(int i = 0; i < this->inputVals.size(); i++)
    {
        cout << this->inputVals[i] << "   ";
    }
    cout << endl;
}

void neuralNetwork::printOutput(){
    int idxOutputLayer = this->numLayers - 1;
    Matrix* activatedOutputMatrix = this->layers[idxOutputLayer]->convertActivationValsToMatrix();
    cout << "Output Values" << endl;
    for(int i = 0; i < activatedOutputMatrix->getNumCols(); i++){
        cout << activatedOutputMatrix->getMatrixVal(0, i) << "   ";
    }
    cout << endl;
}

void neuralNetwork::printTargets(){
    cout << "Target Values" << endl;
    for(int i = 0; i < this->targets.size(); i++){
        cout << this->targets[i] << "   ";
    }
    cout << endl;
}

void neuralNetwork::printiterationErrors(){
    cout << "Iteration Errors" << endl;
    for(int i = 0; i < this->iterationErrors.size(); i++){
        cout << this->iterationErrors[i] << "   ";
    }
    cout << endl;
}

neuralNetwork::~neuralNetwork()
{
    for (auto w : this->weightsMatrices) {
        delete w;
    }
    for (auto l : this->layers) {
        delete l;
    }
}