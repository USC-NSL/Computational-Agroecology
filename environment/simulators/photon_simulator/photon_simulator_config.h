#ifndef __PHOTON_SIMULATOR_CONFIG_H__
#define __PHOTON_SIMULATOR_CONFIG_H__

namespace simulator
{

namespace photonsimulator
{

// floating point precision set by this typedef
typedef float real_t;

// TODO: @hangjie please comment this
const int kXAXIS = 0;
const int kYAXIS = 1;
const int kZAXIS = 2;
const int kLEAF = 3;

namespace filepath
{
const char defaultpath[] = "";
const char corn1[] = "Corn1.obj";
const char corn2[] = "Corn2.obj";
const char corn3[] = "Corn3.obj";
const char squash1[] = "Squash1.obj";
const char squash2[] = "Squash2.obj";
const char squash3[] = "Squash3.obj";
} // namespace filepath

} // namespace photonsimulator

} // namespace simulator

#endif // __PHOTON_SIMULATOR_CONFIG_H__