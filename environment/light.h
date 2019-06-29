#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LIGHT_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LIGHT_H

#include <optional>

#include "utility.h"

namespace environment {

// Light related index
struct Light {
  enum LightIntensityCategory {
    VERY_BRIGHT = 0,
    CLEAR_SKIES,
    CLOUDY_SKIES,
    LIGHT_SHADE,
    HEAVY_SHADE
  };

  // Quote from Barath:
  // Given our discussion about direct (point source) vs. indirect (diffuse
  // source) illumination, maybe we can associate each of these LightIntensity
  // categories with two values between 0 and 1 of the amount of indirect light
  // and amount of light lost to absorption/reflection to space.
  struct LightIntensity {
    LightIntensity(const LightIntensityCategory category,
                   const double indirect_light, const double light_lost);
    const LightIntensityCategory category;
    const double indirect_light;
    const double light_lost;
  };

  Light(const std::optional<MinMaxPair<LightIntensity>>& light_intensity,
        const std::optional<double>& angle);

  static const LightIntensity kLightIntensityVeryBright;
  static const LightIntensity kLightIntensityClearSkies;
  static const LightIntensity kLightIntensityCloudySkies;
  static const LightIntensity kLightIntensityLightShade;
  static const LightIntensity kLightIntensityHeavyShade;

  // TODO: consider whether to use references
  // It seems to make sense to use references but std::optional does not support
  // this natively.
  const std::optional<MinMaxPair<LightIntensity>> light_intensity;

  // TODO: replace it with a angle unit
  const std::optional<double> angle;
};

bool operator==(const Light& lhs, const Light& rhs);
bool operator==(const Light::LightIntensity& lhs,
                const Light::LightIntensity& rhs);

}  // namespace environment
#endif  // COMPUTATIONAL_ENVIRONMENT_AGROECOLOGY_LIGHT_H
