//Created: 29-09-2014
//Modified: Mon 29 Sep 2014 15:16:08 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"../subset_sum.hpp"
#include<gtest/gtest.h>
#include<vector>

TEST(SubsetSumTest, Constructor)
{
  std::vector<int> init_values_valid {2,4,6};
  EXPECT_NO_THROW({
      auto sum = SubsetSum<>(10,init_values_valid);
  });

  std::vector<int> init_values_invalid {2, 6, -1};

  EXPECT_THROW({
      auto sum = SubsetSum<>(10,init_values_invalid);
  }, std::invalid_argument);

  std::vector<int> init_values_invalid_b { 0, 2, 4 };

  EXPECT_THROW({
      auto sum = SubsetSum<>(10,init_values_invalid_b);
  }, std::invalid_argument);
}

TEST(SubsetSumTest, EmptyConstruction)
{
  auto sum = SubsetSum<>{ 10, {} };
  auto summed_result = sum.calculate();

  EXPECT_TRUE(summed_result.empty());
}

TEST(SubsetSumTest, NegativeTarget)
{
  auto sum = SubsetSum<>{ -4, {2} };
  auto summed_result = sum.calculate();

  EXPECT_TRUE(summed_result.empty());
}

TEST(SubsetSumTest, RunsOne)
{
  auto sum = SubsetSum<>{ 2, {2} };
  auto summed_result = sum.calculate();

  EXPECT_EQ(1L, summed_result.size());

  auto expected_res = std::vector<int>{2};

  EXPECT_EQ(expected_res, summed_result.front());
}

TEST(SubsetSumTest, RunsTwo)
{
  auto sum = SubsetSum<>{ 6, {2,4} };
  auto summed_result = sum.calculate();

  EXPECT_EQ(2L, summed_result.size());

  auto expected_res = std::list< std::vector<int> > { {2,2,2}, {2,4} };

  EXPECT_EQ(expected_res, summed_result);
}

TEST(SubsetSumTest, RunsThree)
{
  auto sum = SubsetSum<>{ 10, {2,4,6,8} };
  auto summed_result = sum.calculate();

  ASSERT_EQ(6L, summed_result.size());

  auto expected_res = std::list< std::vector<int> > {
    {2,2,2,2,2},
    {2,2,2,4},
    {2,2,6},
    {2,4,4},
    {2,8},
    {4,6} };

  EXPECT_EQ(expected_res, summed_result);
}

TEST(SubsetSumTest, RunsLong)
{
  auto sum = SubsetSum<>{ 10, {1,3,4,6} };
  auto summed_result = sum.calculate();

  ASSERT_EQ(11L, summed_result.size());

  auto expected_res = std::list< std::vector<int> > {
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,3},
    {1,1,1,1,1,1,4},
    {1,1,1,1,3,3},
    {1,1,1,1,6},
    {1,1,1,3,4},
    {1,1,4,4},
    {1,3,3,3},
    {1,3,6},
    {3,3,4},
    {4,6} };

  EXPECT_EQ(expected_res, summed_result);
}
