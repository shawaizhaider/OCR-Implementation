#include "../../include/utils/read_csv.hpp"
using namespace readData;

vector< vector<double> > readCSV::fetchData(string path) {
  vector< vector<double> > data;

  ifstream infile(path);

  string line;
  while(getline(infile, line)) {
    vector<double>  dRow;
    string          token;
    stringstream    ss(line);

    while(getline(ss, token, ',')) {
      dRow.push_back(stof(token));
    }

    data.push_back(dRow);
  }

  return data;
}

