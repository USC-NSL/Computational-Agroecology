#include "landscape.h"
void LandscapeCell::SetAll(std::string name, double area){
    this -> name_ = name;
    this -> area_ = area;
}

LandscapeCell::LandscapeCell(std::string name, double area){
    this -> name_ = name;
    this -> area_ = area;
}
LandscapeCell::LandscapeCell() {
    this -> name_ = "Default";
    this -> area_ = 300;
}
LandscapeCell::~LandscapeCell() {
    //TODO: Should we keep this? It seems like a very basic debug cout
    //std::cout << "Landscape cell " << this -> name_ << " destroyed\n";
}