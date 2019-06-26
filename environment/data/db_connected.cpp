#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>

#include <pqxx/pqxx>
class databaseConnection
{
public:
    pqxx::connection* conn;
    void SetConnection(){
        conn=new pqxx::connection(
                "username=temp "
                "host=db.corral.tacc.utexas.edu "
                "password=timelione "
                "dbname=temp");

    }

    void Disconnect(){
        conn->disconnect();
    }

    pqxx::result query(std::string strSQL){
        //SetConnection();
        pqxx::work trans(*conn,"trans");

        pqxx::result res=trans.exec(strSQL);

        trans.commit();
        return res;
    }
};

int main()
{
    std::ifstream file("PlantEnvironment.csv");

    pqxx::connection c("dbname=computational_agroecology hostaddr=204.57.3.149 user=postgres password=LOSangeles2019");

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

    std::string line = "";
    // Iterate through each line and split the content using delimeter
    getline(file, line);

    //int n;

    pqxx::work txn(c);
    pqxx::result res;
/*    while( lineStream.good() ) {
        std::string substr;
        getline(lineStream, substr, ',');
        if(substr != "" && substr != "-") {
            vec.push_back(substr);
        } else {
            vec.push_back("0");
        }
    }*/
/*    std::cout << std::endl << vec[0] << std::endl;
    txn.prepared("create")("test4")(std::stoi(vec[3]))(std::stoi(vec[4]))(std::stoi(vec[5]))(std::stoi(vec[6]))(std::stoi(vec[9]))
    (std::stoi(vec[10]))(vec[11])(vec[12])(std::stoi(vec[13]))(std::stoi(vec[14]))(std::stoi(vec[15]))(std::stoi(vec[16]))(vec[17])(vec[18])(std::stod(vec[19]))(std::stod(vec[20]))(std::stod(vec[21]))(std::stod(vec[22]))
    ()()(vec[25])(vec[26])(vec[27])(vec[28])()(std::stoi(vec[31]))(std::stoi(vec[32]))(std::stoi(vec[8]))(std::stoi(vec[7]))(vec[1])(vec[2]).exec();*/

    //std::cout << line << std::endl;
    /*while( lineStream.good() ) {
        std::string substr;
        getline(lineStream, substr, ',');
        if(substr != "") {
            vec.push_back(substr);
            std::cout << line << std::endl;
        } else {
            vec.push_back(" ");
        }
    }*/
   /* while (getline(file, line))
    {
        std::vector<std::string> vec;
        std::stringstream lineStream(line);

        //std::cout << line << std::endl;
        while( lineStream.good() ) {
            std::string substr;
            getline(lineStream, substr, ',');
            if(substr != "" && substr != "-") {
                vec.push_back(substr);
            } else {
                vec.push_back("0");
            }
        }*/
        //std::cout << std::endl << vec[0] << std::endl;
        res = txn.prepared("read_by_name")("Abelmoschus esculentus").exec();

       if (res.size() != 0)
        {
            /*txn.prepared("create")(vec[0])(std::stoi(vec[3]))(std::stoi(vec[4]))(std::stoi(vec[5]))(std::stoi(vec[6]))(std::stoi(vec[9]))
                    (std::stoi(vec[10]))(vec[11])(vec[12])(std::stoi(vec[13]))(std::stoi(vec[14]))(std::stoi(vec[15]))(std::stoi(vec[16]))(vec[17])(vec[18])(std::stod(vec[19]))(std::stod(vec[20]))(std::stod(vec[21]))(std::stod(vec[22]))
                    ()()(vec[25])(vec[26])(vec[27])(vec[28])()(std::stoi(vec[31]))(std::stoi(vec[32]))(std::stoi(vec[8]))(std::stoi(vec[7]))(vec[1])(vec[2]).exec();
        */
            std::cout << std::endl << res[0][0] << std::endl;
        }

        //dataList.push_back(vec);
    //}
    // Close the File
    //file.close();


    txn.commit();

    {


        /*std::string test_name = "test name 1";
        int test_temp = 40;

        // --- Create ---
        {
            //txn.prepared("create")(test_name)()
              //      .exec();

            res = txn.prepared("read_by_name")(test_name).exec();

            if (res.size() == 0)
            {
                std::cerr <<
                          "No records with name " << test_name << ", were found." <<
                          std::endl;

                return -1;
            }

            std::string plant_name = res[0][0].as<std::string>();
            assert(plant_name == test_name);

            std::cout << "Plant #" << plant_name << std::endl;
        }

        txn.commit();*/
    }
    c.disconnect();
}

