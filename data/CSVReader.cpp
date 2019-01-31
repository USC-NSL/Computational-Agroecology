//
// Created by xiao on 1/31/19.
//

#include "CSVReader.h"

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
    std::ifstream file(fileName.c_str());

    std::vector<std::vector<std::string> > dataList;

    std::string line = "";
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        std::vector<std::string> vec;
        std::stringstream lineStream(line);

        while( lineStream.good() ) {
            std::string substr;
            getline(lineStream, substr, ',');
            if(substr != "") {
                vec.push_back(substr);
            } else {
                vec.push_back(" ");
            }
        }
        dataList.push_back(vec);
    }
    // Close the File
    file.close();
    return dataList;
}

//int main() {
//    // Creating an object of CSVWriter
//    CSVReader reader("PlantEnvironment.csv");
//
//    // Get the data from CSV File
//    std::vector<std::vector<std::string> > dataList = reader.getData();
//    std::cout << dataList[3][2] << std::endl;
//    std::cout << dataList[dataList.size() - 2][0] << dataList.size() - 1 << std::endl;
//    return 1;
//}