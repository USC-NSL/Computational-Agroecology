#include <gtest/gtest.h>

#include "plant_data_connector.h"

using namespace database;

TEST(ConfigTest, search) {
  PlantDataConnector c;
  EXPECT_EQ(c.QueryRecordByName("Abelmoschus esculentus"), true);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
