#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_

#include <memory>
#include <string>

#include "KDTree/KDTree.hpp"

#include "environment/coordinate.h"
#include "environment/plant.h"

namespace environment {

class PlantContainer {
 public:
  // Types
  using iterator = std::vector<std::unique_ptr<Plant>>::iterator;
  using const_iterator = std::vector<std::unique_ptr<Plant>>::const_iterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reference = std::unique_ptr<Plant> &;
  using const_reference = const std::unique_ptr<Plant> &;
  using pointer = std::unique_ptr<Plant> *;
  using const_pointer = const std::unique_ptr<Plant> *;
  using value_type = std::unique_ptr<Plant>;
  using size_type = std::vector<std::unique_ptr<Plant>>::size_type;

  PlantContainer() : plants_(), kdtree_(){};

  // Fetches a plant pointer specified by the `coordinate` here.
  // Returns `nullptr` if no plant is found.
  Plant *operator[](const Coordinate &coordinate);
  const Plant *operator[](const Coordinate &coordinate) const;

  // The returned pointer belongs to this class. The caller should not free it.
  Plant *AddPlant(const std::string &plant_name, const Coordinate &coordinate,
                  const Meteorology &meteorology);
  bool DelPlant(const Plant &plant);
  bool DelPlant(const Coordinate &coordinate);

  // The returned pointer belongs to this class. The caller should not free it.
  Plant *GetPlant(const Coordinate &coordinate);
  const Plant *GetPlant(const Coordinate &coordinate) const;

  // capacity
  size_type size() const { return plants_.size(); }
  bool empty() const { return plants_.empty(); }

  // iterators
  iterator begin();
  const_iterator begin() const;
  const_iterator cbegin() const;
  iterator end();
  const_iterator end() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;
  const_reverse_iterator crbegin() const;
  reverse_iterator rend();
  const_reverse_iterator rend() const;
  const_reverse_iterator crend() const;

 private:
  bool CheckPosition(const Coordinate &position, const double size);
  void ConstructPlantKDTree();

  std::vector<std::unique_ptr<Plant>> plants_;
  std::unique_ptr<KDTree> kdtree_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
