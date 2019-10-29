#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "environment/coordinate.h"
#include "environment/meteorology.h"
#include "environment/soil.h"
#include "environment/plant_radiation.h"
#include "environment/water_balance.h"

namespace environment {

// Static properties as used for parameters in plant models.
// All values are stored using integer types to avoid floating-point error.
//
// Temperature is in millidegrees Celsius (i.e., 12 C == 12000).
// GDD units are in millidegrees Celsius x days.
// Photo period is in seconds per day.
// Yield is in grams.
//
enum class PlantProperty {
  GDD_BASE_TEMPERATURE = 0,    // Temperature above which to accumulate GDD.
  MIN_ABSOLUTE_TEMPERATURE,    // Lower killing temperature for mature growth.
  MAX_ABSOLUTE_TEMPERATURE,    // Upper killing temperature for mature growth.
  MIN_NEW_GROWTH_TEMPERATURE,  // Lower killing temperature for new growth.
  MAX_NEW_GROWTH_TEMPERATURE,  // Upper killing temperature for new growth.
  MIN_PHOTO_PERIOD,            // Minimum photo period for growth.
  MAX_PHOTO_PERIOD,            // Maximum photo period for growth.
  MAX_HARVEST_YIELD,           // Maximum yield per cropping period.
  GDD_UNITS_AFTER_FULL_BLOOM,  // GDD units needed from full bloom to harvest.
  NUM_PROPERTIES
};

// A collection of plant parameters for specializing a plant model.
using PlantParams = std::unordered_map<PlantProperty, int64_t>;

// Default parameters for all plants.
const PlantParams kDefaultParams = {
    {PlantProperty::GDD_BASE_TEMPERATURE, 20000},         // 20 C.
    {PlantProperty::MIN_ABSOLUTE_TEMPERATURE, 0},         // 0 C.
    {PlantProperty::MAX_ABSOLUTE_TEMPERATURE, 50000},     // 50 C.
    {PlantProperty::MIN_NEW_GROWTH_TEMPERATURE, 10000},   // 10 C.
    {PlantProperty::MAX_NEW_GROWTH_TEMPERATURE, 40000},   // 40 C.
    {PlantProperty::MIN_PHOTO_PERIOD, 0},                 // 0 s.
    {PlantProperty::MAX_PHOTO_PERIOD, 72000},             // 20 hours.
    {PlantProperty::MAX_HARVEST_YIELD, 100000},           // 100 kg.
    {PlantProperty::GDD_UNITS_AFTER_FULL_BLOOM, 1000000}  // 1000 degree-days.
};

class PlantBuilder;  // Forward reference.

// Represents a single plant.
class Plant {
 public:
  // Generic maturity thresholds for use by models as they see fit.
  enum Maturity { SEED = 0, SEEDLING, JUVENILE, MATURE, OLD };

  // The minimum and maximum (unit-less) health values.
  static const int kMinHealth = 0;   // Corresponds to a dead plant.
  static const int kMaxHealth = 10;  // Corresponds to maximial growth.

  virtual ~Plant() {}

  // Harvest this plant. This should return the value of yield.
  int Harvest();

  // TODO: Specify this function more completely and implement in subclasses.
  // Given the `available` resources, which should include all soil and non-soil
  // (e.g., light) resources, attempts to perform growing in the specified
  // number time step(s). Modifies this and returns the resources consumed,
  // which should be component-wise less than `available`.  Performs no
  // accounting in the environment; such accounting is the responsibility of the
  // caller.
  // TODO: Figure out a way to trigger this function every timestep.
  virtual Resources GrowStep(const int64_t num_time_step,
                             const Resources &available) = 0;

  const std::string &name() const { return name_; }

  const Coordinate &position() const { return position_; }

  double trunk_size() const { return trunk_size_; }
  // TODO: deprecate SetTrunkSize() later. It should not be public. Remember to
  // modify the test as well.
  void set_trunk_size(const double trunk_size) { trunk_size_ = trunk_size; }

  double root_size() const { return root_size_; }
  void set_root_size(const double root_size) { root_size_ = root_size; }

  int health() const { return health_; }
  bool flowering() const { return flowering_; }
  double height() const { return height_; }
  int accumulated_gdd() const { return accumulated_gdd_; }
  Maturity maturity() const { return maturity_; }
  int produce() const { return produce_; }
  const PlantParams &params() const { return params_; }

  void set_water_content(WaterBalance::DailyWaterContentReturn water_content) {
    water_content_ = water_content;
  }

  WaterBalance::DailyWaterContentReturn get_water_content() {
    return water_content_;
  }

 protected:
  // Constructs a generic plant with default values, only for child class use.
  Plant(const std::string &name, const double trunk_size = 0.0,
        const double root_size_ = 0.0)
      : name_(name),
        position_(),
        trunk_size_(trunk_size),
        root_size_(root_size_),
        health_(kMaxHealth),
        flowering_(false),
        accumulated_gdd_(0),
        maturity_(SEED),
        produce_(0),
        params_(kDefaultParams),
        leaf_index_area_(1) { }  // TODO: Set this value for leaf_index_area cleanly

  void SetMeteorology(const Meteorology &meteorology) {
    meteorology_ = &meteorology;
    plant_radiation_ = new PlantRadiation(leaf_index_area_, *meteorology_);
  }

  // Overrides internal parameters with the given `params`.
  void SetParams(const PlantParams &params) {
    for (const auto &kv : params) {
      params_[kv.first] = kv.second;
    }
  }

  PlantRadiation* GetPlantRadiation() const {
    return plant_radiation_;
  }

 private:
  friend class PlantBuilder;
  friend class PlantContainer;

  // A descriptive string for this plant (e.g., "avocado").
  std::string name_;

  Coordinate position_;

  // Trunk size of the plant
  double trunk_size_;
  // Root size or canopy size
  double root_size_;

  // Health of the plant in range [kMinHealth, kMaxHealth].
  int health_;

  // Is the plant currently flowering?
  bool flowering_;

  // The height of this plant
  double height_;

  // Accumulated Growing Degree Days.
  // This accumulated value can be reset by the plant model as needed
  int accumulated_gdd_;

  // The plant's current maturity.
  Maturity maturity_;

  // The current total weight of ripen fruit/vegetable/special crop in the
  // single crop in grams.
  int produce_;

  // A collection of static parameters of this plant.
  PlantParams params_;

  PlantRadiation* plant_radiation_;

  const Meteorology* meteorology_;

  const double leaf_index_area_;

  WaterBalance::DailyWaterContentReturn water_content_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_H_
