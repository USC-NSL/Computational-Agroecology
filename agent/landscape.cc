#include "landscape.h"
void LandScapeCell::SetAll(std::string name, double area){
    this -> name = name;
    this -> area = area;
}

LandscapeCell::LandscapeCell(std::string name, double area){
    this -> name = name;
    this -> area = area;
}
LandscapeCell::LandscapeCell() {
    this -> name_ = "Default";
    this -> area_ = 300;
}
LandscapeCell::~LandscapeCell() {
    std::cout << "Landscape cell " << this -> name_ << " destroyed\n";
}