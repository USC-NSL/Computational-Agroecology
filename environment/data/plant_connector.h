#ifndef COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H
#define COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H

#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <pqxx/pqxx>

namespace database {

// Connection to Plant table
class PlantConnector {
public:
    pqxx::connection c("dbname=computational_agroecology hostaddr=204.57.3.149 user=postgres password=LOSangeles2019");

    void SetConnection();
    void Disconnect();
    bool QueryRecordByName(std::string name);
    bool DeleteRecordByName(std::string name);
};
} // namespace database

#endif //COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H
