#ifndef __PHOTON_SIMULATOR_CONFIG_H__
#define __PHOTON_SIMULATOR_CONFIG_H__

#include "photon/photon.h"
#include "photon/neighbor.h"
#include "model/model.h"

namespace simulator{

namespace photonsimulator{

const int kXAXIS = 0;
const int kYAXIS = 1;
const int kZAXIS = 2;
const int kLEAF = 3;
// floating point precision set by this typedef
typedef float real_t;

} // namespace photonsimulator

} // namespace simulator

#endif // __PHOTON_SIMULATOR_CONFIG_H__