// // Ralph: refer to the google coding style to see what this should be
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

namespace simulator {

namespace photonsimulator {

struct Material {
  // const parameters
  Material(float aborption = 0.5, float reflection = 0.5,
           float transmision = 0.0)
      : aborption(aborption),
        reflection(reflection),
        transmision(transmision) {}
  // Ralph: remove this
  ~Material(){};
  float aborption;
  float reflection;
  float transmision;
};

}  // namespace photonsimulator

}  // namespace simulator

#endif /* __MATERIAL_H__ */