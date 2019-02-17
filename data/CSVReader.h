// Copyright 2019
#ifndef DATA_CSVREADER_H_
#define DATA_CSVREADER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "../common/plant_type.h"

class CSVReader {
    std::string fileName;
    std::string delimeter;

 public:
    CSVReader(std::string filename, std::string delm = ","):
        fileName(filename),
        delimeter(delm) { }

    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string> > getData();

    // Function that transfer the data into plant type holder
    std::vector<PlantType> getPlantTypes();
};

#endif  // DATA_CSVREADER_H_
