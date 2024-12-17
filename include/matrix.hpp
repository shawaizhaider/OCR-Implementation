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
        // settters
        void setMatrixVal(int rowIndex, int colIndex, double val);
        // getters
        double getMatrixVal(int rowIndex, int colIndex);
        double generateRandomVal();
        ~Matrix();    
};

#endif // MATRIX_HPP