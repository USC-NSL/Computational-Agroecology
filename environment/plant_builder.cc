#include "plant_builder.h"

#include "plant.h"

namespace environment {

bool PlantBuilder::RegisterPlant(const std::string &model_name,
                                 const PlantGenerator &generator) {
  if (!models_.count(model_name)) {
    models_[model_name] = generator;
    return true;
  }
  return false;
}

void PlantBuilder::UnregisterPlant(const std::string &model_name) {
  models_.erase(model_name);
}

Plant *PlantBuilder::NewPlant(const std::string &model_name) {
  return NewPlant(model_name, PlantParams());
}

Plant *PlantBuilder::NewPlant(const std::string &model_name,
                              const PlantParams &overrides) {
  if (models_.count(model_name)) {
    Plant *p = models_[model_name]();
    p->SetParams(overrides);
    return p;
  }
  return nullptr;
}

// Definition
std::unordered_map<std::string, PlantGenerator> PlantBuilder::models_;

}  // namespace environment