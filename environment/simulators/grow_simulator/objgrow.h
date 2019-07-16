#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_OBJGROW_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_OBJGROW_H_

#include <list>

#include "plant_root.h"

namespace simulator {

namespace growsimulator {

const double kPI = 3.141592653589793238462643383279;
const double kMin = 0.00000001;

class ObjGrow {
public:
	ObjGrow(
		const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
		const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
		const double length);
	void PlantGrow();
	void PlantGrow(
		const double rightHorizontalDegree, const double rightVerticalDegree, const double rightGrowLengthRatio,
		const double leftHorizontalDegree, const double leftVerticalDegree, const double leftGrowLengthRatio,
		const double length);
	void ExportObj(const char* pathname, double width);
private:
	double rightHorizontalDegree_, rightVerticalDegree_, rightGrowLengthRatio_;
	double leftHorizontalDegree_, leftVerticalDegree_, leftGrowLengthRatio_;
	double DegreeToRadian(const double degree) const;
	double RadianToDegree(const double radian) const;
	Vector3 getLeftDir(const PlantRoot& plantroot, double HorizontalDegree, double VerticalDegree);
	Vector3 getRightDir(const PlantRoot& plantroot, double HorizontalDegree, double VerticalDegree);
	real_t getDegree(const Vector3& left, const Vector3& right);
	void getMultiRes(real_t* left, real_t* right, real_t* res, const int m, const int n, const int t);
	std::list<int> getListByInt(int a, int b, int c);
	std::list<PlantRoot> plantRootsToGrow;
	std::list<PlantRoot> plantRootsHasGrown;
};

}  // namespace growsimulator

}  // namespace simulator
#endif // !COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_GROW_SIMULATOR_OBJGROW_H_