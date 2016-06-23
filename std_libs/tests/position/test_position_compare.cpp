/*
 * Created: 10-10-2014
 * Modified: Fri 10 Oct 2014 15:10:14 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<position/position_compare.hpp>
#include<position/position_class.hpp>
#include<gtest/gtest.h>

using namespace Position;

TEST(PositionCompareTest, StrictCompareSameSize)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{1,2,4};

  auto comp = StrictCompare{};
  EXPECT_TRUE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));

  p1 += 3;
  EXPECT_FALSE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));

  p2 -= 7;
  EXPECT_TRUE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));
}

TEST(PositionCompareTest, AbsoluteValueCompareSameSize)
{
  auto p1 = pos{1,-2,3};
  auto p2 = pos{1,2,-4};

  auto comp = AbsoluteValueCompare{};
  EXPECT_TRUE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));

  p1 += 3;
  EXPECT_FALSE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));

  p2 -= 7;
  EXPECT_TRUE(comp(p1,p2));
  EXPECT_FALSE(comp(p2,p1));
}
