#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_

#include <initializer_list>
#include <utility>

template <typename T>
struct MinMaxPair {
  MinMaxPair(const T &min, const T &max) : min(min), max(max) {}
  MinMaxPair(const std::pair<T, T> &input)
      : min(input.first), max(input.second) {}
  MinMaxPair(const std::initializer_list<T> &l) {
    if (l.size() >= 2) {
      min = *(l.begin());
      max = *(l.begin() + 1);
    }
  }

  T min;
  T max;
};

template <typename T>
bool operator==(const MinMaxPair<T> &lhs, const MinMaxPair<T> &rhs) {
  return (lhs.min == rhs.min) && (lhs.max == rhs.max);
}

template <typename T>
bool operator!=(const MinMaxPair<T> &lhs, const MinMaxPair<T> &rhs) {
  return !(lhs == rhs);
}

// TODO: this `double` should be replaced with `degC`
using MinMaxTemperature = MinMaxPair<double>;
// TODO: this `double` should be replaced with `minimeter`
using MinMaxRainfall = MinMaxPair<double>;

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_UTILITY_H_
