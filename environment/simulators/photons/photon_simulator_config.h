#ifndef __PHOTON_SIMULATOR_CONFIG_H__
#define __PHOTON_SIMULATOR_CONFIG_H__

namespace simulator {

namespace photonsimulator {

// resolve macro definition in third-party library
#ifdef PI
#undef PI
#endif

// constant for kd-tree
const int kXAXIS = 0;  // use x-axis
const int kYAXIS = 1;  // use y-axis
const int kZAXIS = 2;  // use z-axis
const int kLEAF = 3;   // just leaf

namespace filepath {
const char defaultpath[] = "";
const char corn1[] = "Corn1.obj";
const char corn2[] = "Corn2.obj";
const char corn3[] = "Corn3.obj";
const char squash1[] = "Squash1.obj";
const char squash2[] = "Squash2.obj";
const char squash3[] = "Squash3.obj";
}  // namespace filepath

}  // namespace photonsimulator

}  // namespace simulator

#endif  // __PHOTON_SIMULATOR_CONFIG_H__
