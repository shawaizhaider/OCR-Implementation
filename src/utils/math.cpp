#include "../../include/utils/math.hpp"
using namespace NN;

void utils::Math::multiplyMatrix(Matrix *a, Matrix *b, Matrix *c) {
  for(int i = 0; i < a->getNumRows(); i++) {
    for(int j = 0; j < b->getNumCols(); j++) {
      for(int k = 0; k < b->getNumRows(); k++) {
        double p      = a->getMatrixVal(i, k) * b->getMatrixVal(k, j);
        double newVal = c->getMatrixVal(i, j) + p;
        c->setMatrixVal(i, j, newVal);
      }

      c->setMatrixVal(i, j, c->getMatrixVal(i, j));
    } 
  }
}
Matrix* utils::Math::multiplyTwoMatrix(Matrix *a, Matrix *b) {
  Matrix *c = new Matrix(a->getNumRows(), b->getNumCols(), false);
  for(int i = 0; i < a->getNumRows(); i++) {
    for(int j = 0; j < b->getNumCols(); j++) {
      for(int k = 0; k < b->getNumRows(); k++) {
        double p      = a->getMatrixVal(i, k) * b->getMatrixVal(k, j);
        double newVal = c->getMatrixVal(i, j) + p;
        c->setMatrixVal(i, j, newVal);
      }

      c->setMatrixVal(i, j, c->getMatrixVal(i, j));
    }
  }
  return c;
}
