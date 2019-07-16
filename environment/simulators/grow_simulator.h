#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_H_

#include <vector>

#include "simulator.h"
#include "grow_simulator/objgrow.h"


namespace simulator {

namespace growsimulator {

const double kPI = 3.141592653589793238462643383279;
const double kMin = 0.00000001;

class GrowSimulator : public Simulator {
 public:
  GrowSimulator(
   const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
   const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
   const double length);
  void SimulateToTime(
   environment::Environment* env,
   const std::chrono::system_clock::time_point& time) override;
 private:
   double rightHorizontalDegree, rightVerticalDegree, rightGrowLengthRatio;
   double leftHorizontalDegree, leftVerticalDegree, leftGrowLengthRatio;
   std::vector<ObjGrow> objgrows_;
 };

}  // namespace growsimulator

}  // namespace simulator
#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_H_
