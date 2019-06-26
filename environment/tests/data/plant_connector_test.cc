#include <gtest/gtest.h>

#include "plant_connector.h"

using namespace environment;

TEST(ConfigTest, search) {
EXPECT_EQ(PlantConnector::QueryRecordByName("Abelmoschus esculentus"), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
