#include "plant_connector.h"
namespace database {

void PlantConnector::SetConnection() {
    /*conn = new pqxx::connection("dbname=computational_agroecology"
                                "hostaddr=204.57.3.149"
                                " user=postgres password=LOSangeles2019");
    */
    conn.prepare("create",
                 "INSERT INTO plants (name, \
minoptimaltemperature, maxoptimaltemperature, minabsolutetemperature, maxabsolutetemperature,\
minabsoluterainfall, maxabsoluterainfall, optimalsoilfertility, absolutesoilfertility, maxoptimallatitude,\
minoptimallatitude, maxabsolutelatitude, minabsolutelatitude, optimalsoilsalinity, absolutesoilsalinity,\
minoptimalsoilph, maxoptimalsoilph, maxabsolutesoilph, minabsolutesoilph, optimalsoildrainage, absolutesoildrainage,\
minoptimallightintensity, maxoptimallightintensity, minabsolutelightintensity, maxabsolutelightintensity, climatezone,\
mincropcycle, maxcropcycle, maxoptimalrainfall, minoptimalrainfall, optimalsoildepth, absolutesoildepth ) \
VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22,$23\
  ,$24, $25, $26, $27, $28, $29, $30, $31, $32 )");
    conn.prepare("read_by_name",
                 "SELECT * FROM plants WHERE name = $1");
    conn.prepare("delete_by_name",
                 "DELETE FROM plants WHERE name = $1");

}

void PlantCononector::disconnect() {
    conn->disconnect();
}

bool PlantConnector::QueryRecordByName(std::string& name) {
    pqxx::work txn(conn);
    pqxx::result res;
    res = txn.prepared("read_by_name")(name).exec();
    if (res.size() != 0) {
        return true;
    } else {
        return false;
    }
}

bool PlantConnector::QueryRecordByName(std::string& name) {
    pqxx::work txn(*conn);
    pqxx::result res;
    res = txn.prepared("read_by_name")(name).exec();
    if (res.size() != 0) {
        res = txn.prepared("delete_by_name")(name).exec();
        txn.commit();
        return true;
    } else {
        return false;
    }
}

} // namespace database
