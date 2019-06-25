#include <string>

#include <gtest/gtest.h>

#include "plant_types/plant_type.h"

using namespace environment::plant_type;

TEST(PlantTypeTest, Initialization) {
environment::Soil soil(environment::Soil::CLAY, 6.0, 1.0, 2.0, 3.0, environment::Soil::HIGH, environment::Soil::HIGH);
environment::Light light(environment::Light::VERY_BRIGHT, 1.1);

PlantType plant_type("Abelmoschus esculentus", "x",
                     true, 0.0, MaxMinTemperature(20, 30),
                     MaxMinTemperature(12, 45), MaxMinRainfall(600, 1200),
                     MaxMinRainfall(300, 2500),
                     50,20,50,20,0,0,40,35, soil, soil, light, light,180,50);

EXPECT_EQ("Abelmoschus esculentus", plant_type.type_name);
EXPECT_EQ(MaxMinTemperature(20, 30), plant_type.optimal_temperature);
EXPECT_EQ(MaxMinTemperature(12, 45), plant_type.absolute_temperature);
EXPECT_EQ(MaxMinRainfall(600, 1200), plant_type.optimal_annual_rainfall);
EXPECT_EQ(MaxMinRainfall(300, 2500), plant_type.absolute_annual_rainfall);
EXPECT_EQ(50, plant_type.max_optimal_soil_depth);
EXPECT_EQ(20, plant_type.min_optimal_soil_depth);
EXPECT_EQ(50, plant_type.max_absolute_soil_depth);
EXPECT_EQ(20, plant_type.min_absolute_soil_depth);

EXPECT_EQ(0, plant_type.max_optimal_latitude);
EXPECT_EQ(0, plant_type.min_optimal_latitude);
EXPECT_EQ(40, plant_type.max_absolute_latitude);
EXPECT_EQ(35, plant_type.min_absolute_latitude);

EXPECT_EQ(soil, plant_type.optimal_soil_condition);
EXPECT_EQ(soil, plant_type.absolute_soil_condition);

EXPECT_EQ(light, plant_type.optimal_light_condition);
EXPECT_EQ(light, plant_type.absolute_light_condition);

EXPECT_EQ(180, plant_type.max_crop_cycle);
EXPECT_EQ(50, plant_type.min_crop_cycle);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}