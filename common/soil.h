// Copyright 2019

#ifndef AGROECOLOGY_SOIL_H_
#define AGROECOLOGY_SOIL_H_

class Soil {
public:
  // was enum class
  enum Texture { CLAY = 0, SILT = 1, SAND = 2 };

  Soil(Texture texture, int pH, int salinity, int orgMat, int water)
      : texture_(texture), pH_(pH), salinity_(salinity),
        organic_matter_(orgMat), water_content_(water) {}
  // with fix soil type and other information
  Soil() : texture_(CLAY), pH_(7), water_content_(0.03) {}
  Texture texture() const { return texture_; }
  double pH() const { return pH_; }
  double salinity() const { return salinity_; }
  double organic_matter() const { return organic_matter_; }
  double water_content() const { return water_content_; }
  const double *nutrients() const { return nutrients_; }

  void addWater(double quantity) {
    water_content_ += quantity;
    if (water_content_ < 0)
      water_content_ = 0;
  }
  void addNitrogen(double quantity) {
    nutrients_[0] += quantity;
    if (nutrients_[0] < 0)
      nutrients_[0] = 0;
  }
  void addPhosphorus(double quantity) {
    nutrients_[1] += quantity;
    if (nutrients_[1] < 0)
      nutrients_[1] = 0;
  }
  void addPotassium(double quantity) {
    nutrients_[2] += quantity;
    if (nutrients_[2] < 0)
      nutrients_[2] = 0;
  }
  void addCalcium(double quantity) {
    nutrients_[3] += quantity;
    if (nutrients_[3] < 0)
      nutrients_[3] = 0;
  }
  void addMagnesium(double quantity) {
    nutrients_[4] += quantity;
    if (nutrients_[4] < 0)
      nutrients_[4] = 0;
  }
  void addSulfur(double quantity) {
    nutrients_[5] += quantity;
    if (nutrients_[5] < 0)
      nutrients_[5] = 0;
  }

private:
  Texture texture_;
  double pH_;
  double salinity_;
  double organic_matter_;
  double water_content_;

  // contains nitrogen, phosphorus, potassium, calcium, magnesium, and sulfur
  // information for the soil, contained in the corresponding indices
  double nutrients_[6];
};

#endif
