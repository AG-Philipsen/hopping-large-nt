/*
 * Created: 10-10-2014
 * Modified: Fri 10 Oct 2014 14:58:28 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<position/position_class.hpp>
#include<gtest/gtest.h>

#include"../utility_functions.hpp"

using namespace Position;

TEST(PositionClassTest, DefaultConstructor)
{
  auto p = pos{};

  EXPECT_EQ(0L, p.size());
  EXPECT_TRUE(p.is_empty());
  EXPECT_TRUE(p.is_zero());
}

TEST(PositionClassTest, LengthConstructor)
{
  auto p = pos(6L);

  EXPECT_EQ(6L, p.size());
  EXPECT_FALSE(p.is_empty());
  EXPECT_TRUE(p.is_zero());
}

TEST(PositionClassTest, IteratorConstructor)
{
  auto v_base = std::vector<int>{4,6,2,4};
  auto p = pos{std::begin(v_base), std::end(v_base)};

  EXPECT_EQ(v_base.size(), p.size());
  EXPECT_FALSE(p.is_empty());
  EXPECT_FALSE(p.is_zero());

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v_base, p);
}

TEST(PositionClassTest, InitializerListConstructor)
{
  auto v_ref = std::vector<int>{4,5,2,4};
  auto p = pos{4,5,2,4};

  EXPECT_EQ(4L, p.size());
  EXPECT_FALSE(p.is_empty());
  EXPECT_FALSE(p.is_zero());

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v_ref, p);
}

TEST(PositionClassTest, ElementOperator)
{
  auto p = pos{1,2,3};

  EXPECT_EQ(3L, p.size());
  EXPECT_EQ(2, p[1]);

  for(size_t i = 0; i < 3L; ++i)
    p[i] = 0;

  EXPECT_TRUE(p.is_zero());

  p[6];

  EXPECT_EQ(7L, p.size());
  EXPECT_TRUE(p.is_zero());

  p[6] = 8;

  EXPECT_FALSE(p.is_zero());

  for(size_t i = 0; i < 6; ++i)
    ASSERT_EQ(0,p[i]);

  ASSERT_EQ(8,p[6]);
}

TEST(PositionClassTest, AtFunction)
{
  auto p = pos{1,2,3};

  EXPECT_EQ(1,p.at(0));
  EXPECT_EQ(2,p.at(1));
  EXPECT_EQ(3,p.at(2));

  ASSERT_THROW(p.at(6), std::out_of_range);
}

TEST(PositionClassTest, ResizeBack)
{
  auto p = pos{1};
  auto ref = std::vector<int>{1,0,0,0};

  p.resize_back(4L);

  EXPECT_EQ(4L, p.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p);

  auto p2 = pos{1,2,3,4,5,6};
  auto ref2 = std::vector<int>{1,2,3,4};

  p2.resize_back(4L);

  EXPECT_EQ(4L, p2.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref2, p2);

  p2.resize_back(6L);
  auto ref3 = std::vector<int>{1,2,3,4,0,0};

  EXPECT_EQ(6L, p2.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref3, p2);
}

TEST(PositionClassTest, ResizeFront)
{
  auto p = pos{1};
  auto ref = std::vector<int>{0,0,0,1};

  p.resize_front(4L);

  EXPECT_EQ(4L, p.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p);

  auto p2 = pos{1,2,3,4,5,6};
  auto ref2 = std::vector<int>{3,4,5,6};

  p2.resize_front(4L);

  EXPECT_EQ(4L, p2.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref2, p2);

  p2.resize_front(6L);
  auto ref3 = std::vector<int>{0,0,3,4,5,6};

  EXPECT_EQ(6L, p2.size());
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref3, p2);
}

TEST(PositionClassTest, AdditionEqualSize)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{3,2,1};

  auto p3 = p1 + p2;

  EXPECT_EQ(3L, p3.size());

  auto expected = std::vector<int>{4,4,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p3);
}

TEST(PositionClassTest, AdditionAssignmentEqualSize)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{3,2,1};

  p1 += p2;

  EXPECT_EQ(3L, p1.size());

  auto expected = std::vector<int>{4,4,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p1);
}

TEST(PositionClassTest, AdditionDifferentSize)
{
  auto p1 = pos{1,2,3,4};
  auto p2 = pos{4,3,2};

  auto p3 = p1 + p2;

  EXPECT_EQ(4L, p3.size());

  auto expected = std::vector<int>{5,5,5,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p3);
}

TEST(PositionClassTest, AdditionAssignmentDifferentSize)
{
  auto p1 = pos{1,2,3,4};
  auto p2 = pos{4,3,2};

  p1 += p2;

  EXPECT_EQ(4L, p1.size());

  auto expected1 = std::vector<int>{5,5,5,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected1, p1);

  p2 += p1;

  EXPECT_EQ(4L, p2.size());

  auto expected2 = std::vector<int>{9,8,7,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected2, p2);
}

TEST(PositionClassTest, SubtractionEqualSize)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{3,2,1};

  auto p3 = p1 - p2;

  EXPECT_EQ(3L, p3.size());

  auto expected = std::vector<int>{-2,0,2};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p3);
}

TEST(PositionClassTest, SubtractionAssignmentEqualSize)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{3,2,1};

  p1 -= p2;

  EXPECT_EQ(3L, p1.size());

  auto expected = std::vector<int>{-2,0,2};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p1);
}

TEST(PositionClassTest, SubtractionDifferentSize)
{
  auto p1 = pos{1,2,3,4};
  auto p2 = pos{4,3,2};

  auto p3 = p1 - p2;

  EXPECT_EQ(4L, p3.size());

  auto expected = std::vector<int>{-3,-1,1,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, p3);
}

TEST(PositionClassTest, SubtractionAssignmentDifferentSize)
{
  auto p1 = pos{1,2,3,4};
  auto p2 = pos{4,3,2};

  p1 -= p2;

  EXPECT_EQ(4L, p1.size());

  auto expected1 = std::vector<int>{-3,-1,1,4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected1, p1);

  p2 -= p1;

  EXPECT_EQ(4L, p2.size());

  auto expected2 = std::vector<int>{7,4,1,-4};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected2, p2);
}

TEST(PositionClassTest, IntegerAddition)
{
  auto p1 = pos{1,2,3};
  p1 += 1;

  auto ref = std::vector<int>{2,2,3};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  p1 += -3;
  --ref[2];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  p1 += -5;
  EXPECT_EQ(5L, p1.size());
  
  ref.resize(5L,0);
  --ref[4];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  auto p2 = p1 + 4;
  ++ref[3];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p2);
}

TEST(PositionClassTest, IntegerSubtraction)
{
  auto p1 = pos{1,2,3};
  p1 -= 1;

  auto ref = std::vector<int>{0,2,3};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  p1 -= -3;
  ++ref[2];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  p1 -= -5;
  EXPECT_EQ(5L, p1.size());
  
  ref.resize(5L,0);
  ++ref[4];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p1);

  auto p2 = p1 - 4;
  --ref[3];
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p2);
}

TEST(PositionClassTest, AddZero)
{
  auto p = pos{1,2,3};
  ASSERT_THROW({ p += 0; }, std::logic_error);

  auto ref = std::vector<int>{1,2,3};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p);

  ASSERT_THROW({ p -= 0; }, std::logic_error);
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p);

  EXPECT_THROW({auto p2 = p + 0;}, std::logic_error);
  EXPECT_THROW({auto p2 = p - 0;}, std::logic_error);
}

TEST(PositionClassTest, NegationOperator)
{
  auto p = pos{1,-2,3,0};
  auto np = -p;

  EXPECT_EQ(4L, np.size());

  auto ref = std::vector<int>{-1,2,-3,0};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, np);
}

TEST(PositionClassTest, EqualityOperator)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{1,2,3};

  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 != p2);

  p1.resize_back(8L);
  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 != p2);

  p2.resize_front(8L);
  EXPECT_TRUE(p1 != p2);
  EXPECT_FALSE(p1 == p2);

  p1.resize_front(13L);
  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 != p2);
}

TEST(PositionClassTest, ComparisonOperator)
{
  auto p1 = pos{1,2,3};
  auto p2 = pos{1,2,4};

  EXPECT_TRUE(p1 < p2);

  p1 += 3;
  EXPECT_FALSE(p1 < p2);
  EXPECT_FALSE(p2 < p1);

  p2 -= 7;
  EXPECT_TRUE(p2 < p1);
}

TEST(PositionClassTest, DeleteTrailingZeros)
{
  auto p = pos{0,1,2,0};
  p.resize_back(8L);

  p.deleteTrailingZeros();
  EXPECT_EQ(3L, p.size());

  auto ref = std::vector<int>{0,1,2};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, ref, p);
}
