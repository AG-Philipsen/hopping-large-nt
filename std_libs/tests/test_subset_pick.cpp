/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 16:54:34 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"../subset_pick.hpp"
#include<gtest/gtest.h>
#include"utility_functions.hpp"
#include"prettyprint.hpp"

#include<vector>

using namespace Utility;

template <typename Container>
SubsetPick<typename Container::iterator> 
CreatePick(Container & c, std::size_t num_pick)
{
  return {std::begin(c), std::end(c), num_pick};
}

template <typename Container1, typename Container2>
inline ::testing::AssertionResult 
ComparePickRemainder(const char* expect_expr,
                     const char* actual_expr,
                     Container1 expected,
                     Container2 actual)
{
  using std::begin;
  using std::end;

  if(std::equal(begin(expected), end(expected), actual.rest_begin()))
    return ::testing::AssertionSuccess();

  return ::testing::AssertionFailure()
    << "Value of: " << actual_expr << "\n"
    << "  Actual: " << UnitTest::IteratorToString(actual.rest_begin(), actual.rest_end()) << "\n"
    << "Expected: " << expect_expr << "\n"
    << "Which is: " << expected;
}

TEST(SubsetPickTest, VectorConstructor)
{
  auto v = std::vector<int>{1,2,3};
  auto v_save = v;

  SubsetPick<std::vector<int>::iterator>{std::begin(v), std::end(v), 1};

  ASSERT_EQ(3L, v.size());
  EXPECT_EQ(v_save, v);
}

TEST(SubsetPickTest, EmptyVector)
{
  auto v = std::vector<int>{};
  auto subset = CreatePick(v,0);

  EXPECT_FALSE(!subset);
  EXPECT_TRUE(static_cast<bool>(subset));

  EXPECT_EQ(std::begin(subset), std::end(subset));
}

TEST(SubsetPickTest, PickAll)
{
  auto v = std::vector<int>{1,2,3};
  auto subset = CreatePick(v, v.size());

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v, subset);

  EXPECT_TRUE(!subset);
  EXPECT_FALSE(static_cast<bool>(subset));

  EXPECT_EQ(subset.rest_begin(), subset.rest_end());

  ++subset;

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v, subset);

  EXPECT_FALSE(!subset);
  EXPECT_TRUE(static_cast<bool>(subset));

  EXPECT_EQ(subset.rest_begin(), subset.rest_end());
}

TEST(SubsetPickTest, PickNone)
{
  auto v = std::vector<int>{1,2,3};
  auto subset = CreatePick(v,0);

  EXPECT_FALSE(!subset);
  EXPECT_TRUE(static_cast<bool>(subset));

  EXPECT_EQ(std::begin(subset), std::end(subset));
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v, subset);
}

TEST(SubsetPickTest, Iterators)
{
  auto v = std::vector<int>{1,2,3};
  auto v_copy = v;
  auto subset = CreatePick(v,2);

  auto v1 = std::vector<int>{1,2};
  auto v2 = std::vector<int>{3};

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);
  EXPECT_EQ(v_copy,v);

  ++subset;

  std::swap(v1[1],v2[0]);

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);
  EXPECT_EQ(v_copy,v);

  ++subset;

  std::swap(v1[0], v2[0]);

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);
  EXPECT_EQ(v_copy,v);
}

TEST(SubsetPickTest, IteratorEdit)
{
  auto v = std::vector<int>{1,2,3};
  auto v_copy = v;
  auto subset = CreatePick(v,2);

  *(subset.begin()) = 5;
  EXPECT_NE(v_copy, v);

  *(subset.rest_begin()) = 8;
  EXPECT_NE(v_copy, v);

  auto v1 = std::vector<int>{5,2};
  auto v2 = std::vector<int>{8};

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);

  ++subset;

  std::swap(v1[1],v2[0]);

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);

  ++subset;

  std::swap(v1[0], v2[0]);

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, subset);
  EXPECT_PRED_FORMAT2(ComparePickRemainder, v2, subset);

  ++subset;

  EXPECT_FALSE(!subset);
  EXPECT_TRUE(static_cast<bool>(subset));
}
