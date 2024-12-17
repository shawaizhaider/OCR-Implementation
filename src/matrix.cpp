#include "../include/matrix.hpp"
// TODO: Implement Matrix class
Matrix::Matrix(int numRows, int numCols, bool randomize)
{
    this->numRows = numRows;
    this->numCols = numCols;
    for(int i = 0; i < numRows; i++)
    {
        vector<double> row;
        for(int j = 0; j < numCols; j++)
        {
            if(randomize)
            {
                row.push_back(generateRandomVal());
            }
            else
            {
                row.push_back(0.00);
            }
        }
        this->matrixVals.push_back(row);
    }
}

double Matrix::generateRandomVal()
{
    return (double)rand() / RAND_MAX;
}


