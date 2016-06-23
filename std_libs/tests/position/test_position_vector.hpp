/*
 * Created: 14-10-2014
 * Modified: Tue 14 Oct 2014 12:37:34 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef TEST_POSITION_VECTOR_HPP
#define TEST_POSITION_VECTOR_HPP

#include<position/position_vector.hpp>
#include<gtest/gtest.h>
#include<vector>

namespace Position {

class PositionVectorTest : public ::testing::Test
{
protected:

  virtual void SetUp()
  {
    pvec.push_back(p1);
    pvec.push_back(p2);
    pvec.push_back(p3);

    pvec2.push_back(p1);
  };

  PositionVectorTest()
    : p1{1},
      p2{0,0,1},
      p3{-1,0,1}
  {};

  pos p1, p2, p3;

  std::vector<pos> pvec;
  std::vector<pos> pvec2;
};

}; //Namespace Position

#endif /* TEST_POSITION_VECTOR_HPP */
