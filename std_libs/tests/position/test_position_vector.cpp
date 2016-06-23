/*
 * Created: 14-10-2014
 * Modified: Tue 14 Oct 2014 13:07:04 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"test_position_vector.hpp"
#include"../utility_functions.hpp"

namespace Position {

TEST_F(PositionVectorTest, Constructor)
{
  auto vec_copy = pvec;
  Vector v {pvec};

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, vec_copy, pvec);
}

TEST_F(PositionVectorTest, SizeAndEmpty)
{
  Vector v {pvec};

  EXPECT_EQ(pvec.size(), v.size());
  EXPECT_FALSE(pvec.empty());
}

TEST_F(PositionVectorTest, Iterator)
{
  Vector v {pvec};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, pvec, v);
}

TEST_F(PositionVectorTest, AccessOperator)
{
  Vector v {pvec};

  EXPECT_EQ(p1, v[0L]);
  EXPECT_EQ(p2, v[1L]);
  EXPECT_EQ(p3, v[2L]);
}

TEST_F(PositionVectorTest, AtFunction)
{
  Vector v {pvec};

  EXPECT_EQ(p1, v.at(0L));
  EXPECT_EQ(p2, v.at(1L));
  EXPECT_EQ(p3, v.at(2L));

  ASSERT_THROW(v.at(3L), std::out_of_range);
}

TEST_F(PositionVectorTest, PushBack)
{
  auto initial_size = pvec.size();

  auto vec_copy = pvec;
  Vector v {vec_copy};

  v.push_back(p1);

  EXPECT_EQ(initial_size + 1, v.size());
  EXPECT_EQ(initial_size + 1, vec_copy.size());

  vec_copy.pop_back();

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, pvec, v);
}

TEST_F(PositionVectorTest, PushBackIterator)
{
  auto initial_size = pvec.size();
  Vector init_vec {pvec};

  auto vec_copy = pvec;
  Vector v {vec_copy};

  v.push_back_iterator(init_vec.begin());

  EXPECT_EQ(initial_size + 1, vec_copy.size());
  EXPECT_EQ(initial_size + 1, v.size());

  auto vec_copy_2 = pvec;
  pvec.push_back(pvec.front());

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, vec_copy_2, v);
}

TEST_F(PositionVectorTest, Clone)
{
  auto initial_size = pvec.size();
  auto vec_copy = pvec;

  Vector v {vec_copy};
  auto v_clone = v.clone();

  v_clone->push_back(p1);

  EXPECT_EQ(initial_size + 1, vec_copy.size());
  EXPECT_EQ(initial_size + 1, v.size());
  EXPECT_EQ(initial_size + 1, v_clone->size());

  vec_copy.pop_back();

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, pvec, *v_clone);
}

TEST_F(PositionVectorTest, MergeIterator)
{
  Vector v1 {pvec};
  Vector v2 {pvec2};

  EXPECT_ANY_THROW(v1.merge_iterator(v2.begin(), v2.end()));
}

}; //Namespace Position
