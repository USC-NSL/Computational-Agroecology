#include "light.h"

namespace environment {

    Light::Light(const LightIntensity light_intensity, const double angle)
            : angle(angle),
              light_intensity(light_intensity){}

    bool operator==(const Light& lhs, const Light& rhs) {
        return (lhs.light_intensity == rhs.light_intensity) &&
               (lhs.angle == rhs.angle);
    }

}  // namespace environment