#include "light.h"

namespace environment {

Light::Light(const std::optional<MinMaxPair<LightIntensity>>& light_intensity,
             const std::optional<double>& angle)
    : light_intensity(light_intensity), angle(angle) {}

Light::LightIntensity::LightIntensity(const LightIntensityCategory category,
                                      const double indirect_light,
                                      const double light_lost)
    : category(category),
      indirect_light(indirect_light),
      light_lost(light_lost) {}

const Light::LightIntensity Light::kLightIntensityVeryBright(Light::VERY_BRIGHT,
                                                             0.0, 0.0);
const Light::LightIntensity Light::kLightIntensityClearSkies(Light::CLEAR_SKIES,
                                                             0.0, 0.0);
const Light::LightIntensity Light::kLightIntensityCloudySkies(
    Light::CLOUDY_SKIES, 0.0, 0.0);
const Light::LightIntensity Light::kLightIntensityLightShade(Light::LIGHT_SHADE,
                                                             0.0, 0.0);
const Light::LightIntensity Light::kLightIntensityHeavyShade(Light::HEAVY_SHADE,
                                                             0.0, 0.0);

bool operator==(const Light& lhs, const Light& rhs) {
  return (lhs.light_intensity == rhs.light_intensity) &&
         (lhs.angle == rhs.angle);
}

bool operator==(const Light::LightIntensity& lhs,
                const Light::LightIntensity& rhs) {
  return (lhs.category == rhs.category) &&
         (lhs.indirect_light == rhs.indirect_light) &&
         (lhs.light_lost == rhs.light_lost);
}

}  // namespace environment