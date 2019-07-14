#include "plant_data_connector.h"

#include <iostream>

namespace database {

PlantDataConnector::PlantDataConnector(const std::string &db_name,
                                       const std::string &hostaddr,
                                       const std::string &user,
                                       const std::string &password)
    : db_name_(db_name),
      hostaddr_(hostaddr),
      user_(user),
      password_(password),
      c_(std::string("dbname=") + db_name + " hostaddr=" + hostaddr +
         " user=" + user + " password=" + password),
      txn_(c_) {}

bool PlantDataConnector::QueryRecordByName(const std::string &name) {
  const std::string kPrepareQueryByName = "query_by_name";
  c_.prepare(kPrepareQueryByName, "SELECT * FROM plants WHERE name = $1");
  auto result = txn_.exec_prepared(kPrepareQueryByName, name);

  if (result.empty()) {
    return false;
  }

  for (const auto &entry : result) {
    for (const auto &col : entry) {
      std::cout << col << ' ';
    }
    std::cout << '\n';
  }

  return true;
}

}  // namespace database
