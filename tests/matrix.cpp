#include <iostream>
#include <cstdlib>
#include "../include/matrix.hpp"

using namespace std;

int main(int argc, char const **argv)
{
    #ifdef _WIN32
        system("cls");
    #else
    system("clear");
    #endif

    /*Testing Matrix Class*/
    Matrix *matrix = new Matrix(3,2,true);
    matrix->printMatrix();

    cout << "Transposed Matrix" << endl;
    Matrix *transposedMatrix = matrix->transpose();
    transposedMatrix->printMatrix();

    delete matrix;
    delete transposedMatrix;
    
    return 0;
}
