#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_BUILDER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_BUILDER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "environment/plant.h"

namespace environment {

// A generator function for constructing a new plant object.
using PlantGenerator = std::function<Plant *()>;

// TODO: Create a public method to show all available plant names.
// A non-instantiable class to construct plant objects by model/spec.
class PlantBuilder {
 public:
  // Registers the given plant generation function `generator` under the given
  // `model_name` if not already registered.  Returns true upon success.
  static bool RegisterPlant(const std::string &model_name,
                            const PlantGenerator &generator);

  // Unregisters the given model name, if present.
  static void UnregisterPlant(const std::string &model_name);

  // Constructs and returns a new Plant object using the generator registered
  // under `model_name` and passes `overrides` to the generator.  If no model is
  // found or an error occurs, returns nullptr.  Does not retain ownership of
  // the returned pointer.
  static Plant *NewPlant(const std::string &model_name,
                         const Meteorology &meteorology);
  static Plant *NewPlant(const std::string &model_name,
                         const Meteorology &meteorology,
                         const PlantParams &overrides);

  // Returns a list of all registered plant model names
  static std::vector<std::string> GetPlantList();

 private:
  PlantBuilder() {}

  // Maps from registered plant model name strings to generator functions for
  // constructing Plant objects of that type.
  static std::unordered_map<std::string, PlantGenerator> models_;
};

#define DEF_PLANT(_PLANT, _NAME)                                          \
  class _PLANT##_generator_class {                                        \
   public:                                                                \
    _PLANT##_generator_class() {                                          \
      PlantBuilder::RegisterPlant(                                        \
          _NAME, PlantGenerator([]() {                                    \
            return new environment::plants::_PLANT(_NAME);                \
          }));                                                            \
    }                                                                     \
    ~_PLANT##_generator_class() { PlantBuilder::UnregisterPlant(_NAME); } \
  };

#define ADD_PLANT(_PLANT, _NAME) \
  DEF_PLANT(_PLANT, _NAME);      \
  static _PLANT##_generator_class _PLANT##_singleton;

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANT_BUILDER_H_
