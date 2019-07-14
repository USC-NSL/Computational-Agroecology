#ifndef COMPUTATIONAL_AGROECOLOGY_PLANT_DATA_CONNECTOR_H
#define COMPUTATIONAL_AGROECOLOGY_PLANT_DATA_CONNECTOR_H

#include <string>

#include <pqxx/pqxx>

namespace database {

class PlantDataConnector {
 public:
  PlantDataConnector(const std::string &db_name = "computational_agroecology",
                     const std::string &hostaddr = "204.57.3.149",
                     const std::string &user = "postgres",
                     const std::string &password = "LOSangeles2019");

  // Experimental function: to see if a species is in the database
  bool QueryRecordByName(const std::string &name);

  // TODO: implement this
  // Import all plant data and register them in the `PlantBuilder`.
  size_t ImportPlantData();

 private:
  std::string db_name_;
  std::string hostaddr_;
  std::string user_;
  std::string password_;

  pqxx::connection c_;
  pqxx::work txn_;
};

}  // namespace database

#endif  // COMPUTATIONAL_AGROECOLOGY_PLANT_DATA_CONNECTOR_H