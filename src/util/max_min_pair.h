#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_

#include <initializer_list>
#include <utility>

template <typename T>
struct MaxMinPair {
  MaxMinPair(const T& max, const T& min) : max(max), min(min) {}
  MaxMinPair(const std::pair<T, T>& input)
      : max(input.first), min(input.second) {}
  MaxMinPair(const std::initializer_list<T>& l) {
    if (l.size() >= 2) {
      max = *(l.begin());
      min = *(l.begin() + 1);
    }
  }

  T max;
  T min;
};

template <typename T>
bool operator==(const MaxMinPair<T>& lhs, const MaxMinPair<T>& rhs) {
  return (lhs.max == rhs.max) && (lhs.min == rhs.min);
}

template <typename T>
bool operator!=(const MaxMinPair<T>& lhs, const MaxMinPair<T>& rhs) {
  return !(lhs == rhs);
}

using MaxMinTemperature = MaxMinPair<double>;
using MaxMinRainfall = MaxMinPair<double>;

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_