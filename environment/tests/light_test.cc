#include <gtest/gtest.h>

#include "light.h"

using namespace environment;

TEST(lightTest, ConstructorTest) {
Light light(Light::VERY_BRIGHT, 1.1);

EXPECT_EQ(Light::VERY_BRIGHT, light.light_intensity);
EXPECT_EQ(1.1, light.angle);
}

TEST(SoilTest, OperatorTest) {
Light lhs(Light::VERY_BRIGHT, 1.1);
Light rhs(Light::VERY_BRIGHT, 1.1);

EXPECT_TRUE(lhs == rhs);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
