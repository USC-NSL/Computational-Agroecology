#include <gtest/gtest.h>

#include "location.h"

using namespace environment;

TEST(LocationTest, ConstructorTest) {
  Location location(100, 200, 300, 400);

  EXPECT_EQ(100, location.longitude_left);
  EXPECT_EQ(200, location.longitude_right);
  EXPECT_EQ(300, location.latitude_top);
  EXPECT_EQ(400, location.latitude_bottom);
}

TEST(LocationTest, OperatorTest) {
  Location lhs(100, 200, 300, 400);
  Location rhs(100, 200, 300, 400);

  EXPECT_TRUE(lhs == rhs);

  Location new_location(500, 200, 300, 400);
  EXPECT_FALSE(lhs == new_location);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
