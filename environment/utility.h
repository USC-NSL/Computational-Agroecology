#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_

#include <initializer_list>
#include <unordered_map>
#include <utility>

// TODO: Complete this resource type enum.
enum class ResourceType {
  NITROGEN = 0,  // TODO.
};

// TODO: Document this Resources map based upon the right units for the value
// type.
using Resources = std::unordered_map<ResourceType, int64_t>;

template <typename T>
struct MinMaxPair {
  MinMaxPair(const T& min, const T& max) : min(min), max(max) {}
  MinMaxPair(const std::pair<T, T>& input)
      : min(input.first), max(input.second) {}
  MinMaxPair(const std::initializer_list<T>& l) {
    if (l.size() >= 2) {
      min = *(l.begin());
      max = *(l.begin() + 1);
    }
  }

  T min;
  T max;
};

template <typename T>
bool operator==(const MinMaxPair<T>& lhs, const MinMaxPair<T>& rhs) {
  return (lhs.min == rhs.min) && (lhs.max == rhs.max);
}

template <typename T>
bool operator!=(const MinMaxPair<T>& lhs, const MinMaxPair<T>& rhs) {
  return !(lhs == rhs);
}

// TODO: this `double` should be replaced with `degC`
using MinMaxTemperature = MinMaxPair<double>;
// TODO: this `double` should be replaced with `minimeter`
using MinMaxRainfall = MinMaxPair<double>;

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_
