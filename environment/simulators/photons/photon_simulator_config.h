#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_PHOTON_SIMULATOR_CONFIG_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_PHOTON_SIMULATOR_CONFIG_H_

namespace simulator {

namespace photonsimulator {

// resolve macro definition in third-party library
#ifdef PI
#undef PI
#endif

// OpenGL rendering coefficients
const size_t kSizeOfVertexBuffer = 3;
const size_t kSizeOfNormalBuffer = 3;
const size_t kSizeOfTexcoordBuffer = 2;
// RGB = 3, RGBA = 4, we use RGB here
const size_t kNumOfChannels = 3;

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

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTON_PHOTON_SIMULATOR_CONFIG_H_
