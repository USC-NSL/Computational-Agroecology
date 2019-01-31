#ifndef COMPUTATIONAL_AGROECOLOGY_CSVREADER_H
#define COMPUTATIONAL_AGROECOLOGY_CSVREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>
// #include <algorithm>
// #include <boost/algorithm/string.hpp>

class CSVReader {
    std::string fileName;
    std::string delimeter;

public:

  CSVReader(std::string filename, std::string delm = ",") :
        fileName(filename), delimeter(delm)
  { }
  // Function to fetch data from a CSV File
  std::vector<std::vector<std::string> > getData();


};


#endif //COMPUTATIONAL_AGROECOLOGY_CSVREADER_H
