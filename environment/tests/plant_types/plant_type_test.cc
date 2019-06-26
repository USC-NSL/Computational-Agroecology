#include <string>

#include <gtest/gtest.h>

#include "plant_types/plant_type.h"

#include "utility.h"

using namespace environment::plant_type;

TEST(PlantTypeTest, Initialization) {
environment::Soil soil(environment::Soil::CLAY, 6.0, 1.0, 2.0, 3.0, environment::Soil::HIGH, environment::Soil::HIGH);
environment::Light light(environment::Light::VERY_BRIGHT, 1.1);

PlantType plant_type("Abelmoschus esculentus", "x", true, 0.0, MaxMinTemperature(20, 30),
        MaxMinTemperature(12, 45), MaxMinRainfall(600, 1200),
                     MaxMinRainfall(300, 2500),
                     MaxMinPair<int>({100, 10}), MaxMinPair<int>({100, 10}),
                     MaxMinPair<int>(100, 10), MaxMinPair<int>(100, 10),
                     soil, soil, light, light, MaxMinPair<int>(180, 50));

EXPECT_EQ("Abelmoschus esculentus", plant_type.type_name);
EXPECT_EQ(MaxMinTemperature(20, 30), plant_type.optimal_temperature);
EXPECT_EQ(MaxMinTemperature(12, 45), plant_type.absolute_temperature);
EXPECT_EQ(MaxMinRainfall(600, 1200), plant_type.optimal_annual_rainfall);
EXPECT_EQ(MaxMinRainfall(300, 2500), plant_type.absolute_annual_rainfall);
EXPECT_EQ(100, plant_type.optimal_soil_depth.max);
EXPECT_EQ(10, plant_type.optimal_soil_depth.min);
EXPECT_EQ(100, plant_type.absolute_soil_depth.max);
EXPECT_EQ(10, plant_type.absolute_soil_depth.min);

EXPECT_EQ(100, plant_type.optimal_latitude.max);
EXPECT_EQ(10, plant_type.optimal_latitude.min);
EXPECT_EQ(100, plant_type.absolute_latitude.max);
EXPECT_EQ(10, plant_type.absolute_latitude.min);

EXPECT_EQ(soil, plant_type.optimal_soil_condition);
EXPECT_EQ(soil, plant_type.absolute_soil_condition);

EXPECT_EQ(light, plant_type.optimal_light_condition);
EXPECT_EQ(light, plant_type.absolute_light_condition);

EXPECT_EQ(180, plant_type.crop_cycle.max);
EXPECT_EQ(50, plant_type.crop_cycle.min);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}