#ifndef AGROECOLOGY_LANDSCAPE_H_
#define AGROECOLOGY_LANDSCAPE_H_
#include "../common/plant.h"
#include "../location.h"

// This represents a grid cell in a land
// TODO (Elizabeth) add more members
class LandscapeCell {
private:
    std::string name_;
    double area_;

public:
    std::string GetName();
    void SetName(std::string name);
    void SetArea(double area);
    void SetAll(std::string name, double area);
    LandscapeCell(std::string , double);
    LandscapeCell();
    ~LandscapeCell();
};

#endif  // AGROECOLOGY_LANDSCAPE_H_