// Copyright 2019

#include "CSVReader.h"

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData() {
    std::ifstream file(fileName.c_str());

    std::vector<std::vector<std::string> > dataList;

    std::string line = "";
    // Iterate through each line and split the content using delimeter
    while ( getline(file, line) ) {
        std::vector<std::string> vec;
        std::stringstream lineStream(line);

        while ( lineStream.good() ) {
            std::string substr;
            getline(lineStream, substr, ',');
            if ( substr != "" ) {
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

std::vector<plantType> getPlantTypes() {
    std::vector<plantType> plantTypesVector;

    // Information about plant and environment are in
    // "../data/PlantEnvironment.csv"
    CSVReader reader("../data/PlantEnvironment.csv");

    // Get the data from CSV File
    std::vector<std::vector<std::string> > dataList = reader.getData();

    // Iterate through all the row and put them into the plant type class
    // plantType(std::string plantName, int MaxOT, int MinOT, int MaxAT,
    // int MinAT, int MaxAR, int MinAR, int MaxOR, int MinOR Max Optimal
    // Temperature, Min Optimal Temperature, Max Absolute Temperature,
    // Min Absolute Temperature, Max Absolute Rainfall,
    // Min Absolute Rainfall, Max Optimal Rainfall, Min Optimal Rainfall
    for ( int i = 1; i != dataList.size(); i++ ) {
        plantType plant(dataList[i][0], atoi(dataList[i][4].c_str()),
                atoi(dataList[i][3].c_str()), atoi(dataList[i][6].c_str()),
                atoi(dataList[i][5].c_str()), atoi(dataList[i][10].c_str()),
               atoi(dataList[i][9].c_str()), atoi(dataList[i][8].c_str()),
               atoi(dataList[i][7].c_str()));

        plantTypesVector.push_back(plant);
    }
    return plantTypesVector;
}