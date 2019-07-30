#include <gtest/gtest.h>

#include <environment/utility.h>

TEST(MinMaxPairTest, ConstructorTest_1) {
  MinMaxPair<int> min_max_pair(10, 100);

  EXPECT_EQ(10, min_max_pair.min);
  EXPECT_EQ(100, min_max_pair.max);
}

TEST(MinMaxPairTest, ConstructorTest_2) {
  auto a_pair = std::make_pair<int, int>(10, 100);
  MinMaxPair<int> min_max_pair(a_pair);

  EXPECT_EQ(10, min_max_pair.min);
  EXPECT_EQ(100, min_max_pair.max);
}

TEST(MinMaxPairTest, ConstructorTest_3) {
  MinMaxPair<int> min_max_pair({10, 100});

  EXPECT_EQ(10, min_max_pair.min);
  EXPECT_EQ(100, min_max_pair.max);

  // should discard extra elements
  min_max_pair = MinMaxPair<int>({10, 100, 5, 4, 3, 2, 1});

  EXPECT_EQ(10, min_max_pair.min);
  EXPECT_EQ(100, min_max_pair.max);
}

TEST(MinMaxPairTest, OperatorTest) {
  MinMaxPair<int> min_max_pair_1({100, 10});
  MinMaxPair<int> min_max_pair_2({200, 20});

  EXPECT_TRUE(min_max_pair_1 != min_max_pair_2);
  EXPECT_FALSE(min_max_pair_1 == min_max_pair_2);

  min_max_pair_1 = min_max_pair_2;

  EXPECT_TRUE(min_max_pair_1 == min_max_pair_2);
  EXPECT_FALSE(min_max_pair_1 != min_max_pair_2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}