#ifndef COMPUTATIONAL_AGROECOLOGY_CSVREADER_H
#define COMPUTATIONAL_AGROECOLOGY_CSVREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "../agent/plantsClassifier/plantType.cpp"

class CSVReader {
    std::string fileName;
    std::string delimeter;

public:
  CSVReader(std::string filename, std::string delm = ",") :
        fileName(filename), delimeter(delm) { }

  // Function to fetch data from a CSV File
  std::vector<std::vector<std::string> > getData();

  // Function that transfer the data into plant type holder
  std::vector<plantType> getPlantTypes();
};

#endif //COMPUTATIONAL_AGROECOLOGY_CSVREADER_H
