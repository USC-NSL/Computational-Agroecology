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
class PlantConnector {
public:
    bool QueryRecordByName(std::string& name);
};
} // namespace database

#endif //COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H
