#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MATERIAL_H_
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MATERIAL_H_

namespace simulator {

namespace photonsimulator {

struct Material {
  // const parameters
  Material(float aborption = 0.5, float reflection = 0.5,
           float transmision = 0.0)
      : aborption(aborption),
        reflection(reflection),
        transmision(transmision) {}
  float aborption;
  float reflection;
  float transmision;
};

}  // namespace photonsimulator

}  // namespace simulator

#endif  // COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_SIMULATORS_PHOTONS_MODEL_MATERIAL_H_