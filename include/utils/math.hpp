#ifndef _MATH_HPP_
#define _MATH_HPP_

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "../matrix.hpp"

using namespace std;

namespace utils
{
  class Math
  {
  public:
    static void multiplyMatrix(NN::Matrix *a, NN::Matrix *b, NN::Matrix *c);
    static NN::Matrix *multiplyTwoMatrix(NN::Matrix *a, NN::Matrix *b);
  };
}

#endif
