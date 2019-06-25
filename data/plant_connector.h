#ifndef COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H
#define COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H

#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <sstream>

class plant_connector {
public:
    pqxx::connection* conn;
    void SetConnection(){
        conn=new pqxx::connection("dbname=computational_agroecology"
                                  "hostaddr=204.57.3.149"
                                  " user=postgres password=LOSangeles2019");
        c.prepare("create",
                  "INSERT INTO plants (name, \
 minoptimaltemperature, maxoptimaltemperature, minabsolutetemperature, maxabsolutetemperature,\
 minabsoluterainfall, maxabsoluterainfall, optimalsoilfertility, absolutesoilfertility, maxoptimallatitude,\
 minoptimallatitude, maxabsolutelatitude, minabsolutelatitude, optimalsoilsalinity, absolutesoilsalinity,\
 minoptimalsoilph, maxoptimalsoilph, maxabsolutesoilph, minabsolutesoilph, optimalsoildrainage, absolutesoildrainage,\
 minoptimallightintensity, maxoptimallightintensity, minabsolutelightintensity, maxabsolutelightintensity, climatezone,\
 mincropcycle, maxcropcycle, maxoptimalrainfall, minoptimalrainfall, optimalsoildepth, absolutesoildepth ) \
    VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22,$23\
              ,$24, $25, $26, $27, $28, $29, $30, $31, $32 )");
        c.prepare("read_by_name",
                  "SELECT * FROM plants WHERE name = $1");
        c.prepare("delete_by_name",
                  "DELETE FROM plants WHERE name = $1");

    }

    void Disconnect(){
        conn->disconnect();
    }

    bool QueryRecordByName(std::string name) {
        pqxx::work txn(*conn);
        pqxx::result res;
        res = txn.prepared("read_by_name")(name).exec();
        if (res.size() == 0)
        {
            return true;
        } else {
            return false;
        }
    }
};


#endif //COMPUTATIONAL_AGROECOLOGY_PLANT_CONNECTOR_H
