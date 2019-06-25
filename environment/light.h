#ifndef COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LIGHT_H
#define COMPUTATIONAL_AGROECOLOGY_ENVIRONMENT_LIGHT_H

namespace environment {

// Light related index
struct Light {
    enum LightIntensity {
        VERY_BRIGHT = 0, CLEAR_SKIES, CLOUDY_SKIES, LIGHT_SHADE, HEAVY_SHADE
    };

    Light(const LightIntensity light_intensity, const double angle);

    const LightIntensity light_intensity;
    const double angle;
};

bool operator==(const Light& lhs, const Light& rhs);

}
#endif //COMPUTATIONAL_ENVIRONMENT_AGROECOLOGY_LIGHT_H
