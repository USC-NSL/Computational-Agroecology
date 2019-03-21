#include "../location.h"


class Config {

public:



	Location getLoc() { return loc;}
	std::vector<PlantType> getPlants()  {return plants;}


private:


	Location loc; // latitude and longitude of location
	std::vector<PlantType> plants; // all available plants to use





}