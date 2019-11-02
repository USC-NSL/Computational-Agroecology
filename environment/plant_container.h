#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_

#include <iterator>
#include <memory>
#include <string>

#include "KDTree/KDTree.hpp"

#include "environment/coordinate.h"
#include "environment/plant.h"

namespace environment {

class PlantContainer {
 public:
  // Types
  class iterator;
  class const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::unique_ptr<Plant> &reference;
  typedef const std::unique_ptr<Plant> &const_reference;
  typedef std::unique_ptr<Plant> *pointer;
  typedef const std::unique_ptr<Plant> *const_pointer;
  typedef std::unique_ptr<Plant> value_type;
  typedef std::vector<std::unique_ptr<Plant>>::size_type size_type;

  PlantContainer() : plants_(), kdtree_(){};

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

class PlantContainer::iterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           std::unique_ptr<Plant>> {
 public:
  iterator() : it_() {}
  iterator(const std::vector<std::unique_ptr<Plant>>::iterator &it) : it_(it) {}
  reference operator*() const { return *it_; }
  pointer operator->() const { return &(*it_); }
  iterator &operator++() {
    ++it_;
    return *this;
  }
  iterator operator++(int) {
    iterator ret = *this;
    ++(*this);
    return ret;
  }
  iterator &operator--() {
    --it_;
    return *this;
  }
  iterator operator--(int) {
    iterator ret = *this;
    --(*this);
    return ret;
  }
  bool operator==(const iterator &rhs) const { return it_ == rhs.it_; }
  bool operator!=(const iterator &rhs) const { return it_ != rhs.it_; }

 private:
  friend class const_iterator;
  std::vector<std::unique_ptr<Plant>>::iterator it_;
};

class PlantContainer::const_iterator
    : public std::iterator<std::bidirectional_iterator_tag,
                           std::unique_ptr<Plant>> {
 public:
  // implicit conversion from iterator to const_iterator
  const_iterator(const PlantContainer::iterator &it) : it_(it.it_) {}

  const_iterator() : it_() {}
  const_iterator(const std::vector<std::unique_ptr<Plant>>::const_iterator &it)
      : it_(it) {}
  const_reference operator*() const { return *it_; }
  const_pointer operator->() const { return &(*it_); }
  const_iterator &operator++() {
    ++it_;
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator ret = *this;
    ++(*this);
    return ret;
  }
  const_iterator &operator--() {
    --it_;
    return *this;
  }
  const_iterator operator--(int) {
    const_iterator ret = *this;
    --(*this);
    return ret;
  }
  bool operator==(const const_iterator &rhs) const { return it_ == rhs.it_; }
  bool operator!=(const const_iterator &rhs) const { return it_ != rhs.it_; }

 private:
  std::vector<std::unique_ptr<Plant>>::const_iterator it_;
};

}  // namespace environment

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_PLANTCONTAINER_H_
