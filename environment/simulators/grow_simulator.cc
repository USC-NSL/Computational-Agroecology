#include "grow_simulator.h"
#include "environment.h"

namespace simulator {

namespace growsimulator {

GrowSimulator::GrowSimulator(
  const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
  const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
  const double length)
	: rightHorizontalDegree_(DegreeToRadian(rightHorizontalDegree)),
	  rightVerticalDegree_(DegreeToRadian(90 - rightVerticalDegree)),
	  rightGrowLengthRatio_(rightGrowLengthRatio),
	  leftHorizontalDegree_(DegreeToRadian(leftHorizontalDegree)),
	  leftVerticalDegree_(DegreeToRadian(90 - leftVerticalDegree)),
	  leftGrowLengthRatio_(leftGrowLengthRatio) {}

void GrowSimulator::SimulateToTime(
  environment::Environment* env,
  const std::chrono::system_clock::time_point& time) {
	for (auto obj : objgrows_) {
	  obj.PlantGrow();
	}
}

}  // namespace growsimulator

}  // namespace simulator