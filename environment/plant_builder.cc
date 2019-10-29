#include "plant_builder.h"

#include "environment/meteorology.h"
#include "environment/plant.h"

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

Plant *PlantBuilder::NewPlant(const std::string &model_name, const Meteorology &meteorology) {
  return NewPlant(model_name, meteorology, PlantParams());
}

Plant *PlantBuilder::NewPlant(const std::string &model_name,
                              const Meteorology &meteorology,
                              const PlantParams &overrides) {
  if (models_.count(model_name)) {
    Plant *p = models_[model_name]();
    p->SetParams(overrides);
    p->SetMeteorology(meteorology);
    return p;
  }
  return nullptr;
}

std::vector<std::string> PlantBuilder::GetPlantList() {
  std::vector<std::string> plant_list;
  plant_list.reserve(models_.size());

  for (const auto &p : models_) {
    plant_list.push_back(p.first);
  }

  return plant_list;
}

// Definition
std::unordered_map<std::string, PlantGenerator> PlantBuilder::models_;

}  // namespace environment
