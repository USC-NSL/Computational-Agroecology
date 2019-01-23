#include "landscape.h"
void LandScapeCell::SetAll(std::string name, std::string plant, double area){
    this -> name = name;
    this -> plant = plant;
    this -> area = area;
}

LandscapeCell::LandscapeCell(std::string name, std::string plant, double area){
    this -> name = name;
    this -> plant = plant;
    this -> area = area;
}
LandscapeCell::LandscapeCell() {
    this -> name_ = "Default";
    this -> plant_ = "Maize";
    this -> area_ = 300;
}
LandscapeCell::~LandscapeCell() {
    std::cout << "Landscape cell " << this -> name_ << " destroyed\n";
}