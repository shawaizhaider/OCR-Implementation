#include "../include/matrix.hpp"
#include <random>
using namespace std;
// TODO: Implement Matrix class
Matrix::Matrix(int numRows, int numCols, bool randomize)
{
    this->numRows = numRows;
    this->numCols = numCols;
    for(int i = 0; i < numRows; i++)
    {
        vector<double> columnVals;
        for(int j = 0; j < numCols; j++)
        {
            if(randomize)
            {
                /*Might need a focus here*/
                columnVals.push_back(generateRandomVal());
            }
            else
            {
                columnVals.push_back(0.00);
            }
        }
        this->matrixVals.push_back(columnVals);
    }
}

void Matrix::printMatrix()
{
    for(int i = 0; i < this->numRows; i++)
    {
        for(int j = 0; j < this->numCols; j++)
        {
            cout << this->matrixVals[i][j] << "   ";
        }
        cout << endl;
    }
}
/*Implement*/
double Matrix::generateRandomVal()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    return dis(gen);

}


Matrix* Matrix::transpose(){
    Matrix *transposedMatrix = new Matrix(this->numCols, this->numRows, false);
    for(int i = 0; i < this->numRows; i++){
        for(int j = 0; j < this->numCols; j++){
            transposedMatrix->setMatrixVal(j, i, this->getMatrixVal(i, j));
        }
    }
    return transposedMatrix;
}

Matrix::~Matrix(){
    //TODO: Implement destructor
}