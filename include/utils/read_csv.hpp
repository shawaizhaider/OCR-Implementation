#ifndef READ_CSV_HPP
#define READ_CSV_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;

namespace readData
{
  class readCSV
  {
  public:
    static vector< vector<double> > fetchData(string path);
  };
}

#endif // READ_CSV_HPP
