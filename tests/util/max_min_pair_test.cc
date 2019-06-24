#include <gtest/gtest.h>

#include <util/max_min_pair.h>

TEST(MaxMinPairTest, ConstructorTest_1) {
  MaxMinPair<int> max_min_pair(100, 10);

  EXPECT_EQ(100, max_min_pair.max);
  EXPECT_EQ(10, max_min_pair.min);
}

TEST(MaxMinPairTest, ConstructorTest_2) {
  auto a_pair = std::make_pair<int, int>(100, 10);
  MaxMinPair<int> max_min_pair(a_pair);

  EXPECT_EQ(100, max_min_pair.max);
  EXPECT_EQ(10, max_min_pair.min);
}

TEST(MaxMinPairTest, ConstructorTest_3) {
  MaxMinPair<int> max_min_pair({100, 10});

  EXPECT_EQ(100, max_min_pair.max);
  EXPECT_EQ(10, max_min_pair.min);

  // should discard extra elements
  max_min_pair = MaxMinPair<int>({100, 10, 5, 4, 3, 2, 1});

  EXPECT_EQ(100, max_min_pair.max);
  EXPECT_EQ(10, max_min_pair.min);
}

TEST(MaxMinPairTest, OperatorTest) {
  MaxMinPair<int> max_min_pair_1({100, 10});
  MaxMinPair<int> max_min_pair_2({200, 20});

  EXPECT_TRUE(max_min_pair_1 != max_min_pair_2);
  EXPECT_FALSE(max_min_pair_1 == max_min_pair_2);

  max_min_pair_1 = max_min_pair_2;

  EXPECT_TRUE(max_min_pair_1 == max_min_pair_2);
  EXPECT_FALSE(max_min_pair_1 != max_min_pair_2);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}