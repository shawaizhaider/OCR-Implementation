#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <math.h>
#include <vector>
#include "neuron.hpp"
using namespace std;

class Matrix{
    private:
        int numRows,numCols;
        vector<vector<double>> matrixVals;
    public:
        Matrix(int numRows, int numcols, bool randomize);
        Matrix* transpose();
        void printMatrix();
        // settters
        void setMatrixVal(int rowIndex, int colIndex, double val){this->matrixVals[rowIndex][colIndex] = val;}
        // getters
        double getMatrixVal(int rowIndex, int colIndex){return this->matrixVals[rowIndex][colIndex];}
        double generateRandomVal();

        int getNumRows(){return this->numRows;}
        int getNumCols(){return this->numCols;}
        ~Matrix();    
};

#endif // MATRIX_HPP