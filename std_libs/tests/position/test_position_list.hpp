/*
 * Created: 10-10-2014
 * Modified: Tue 14 Oct 2014 12:14:29 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef TEST_POSITION_LIST_HPP
#define TEST_POSITION_LIST_HPP

#include<position/position_list.hpp>
#include<gtest/gtest.h>
#include<list>

namespace Position {

class PositionListTest : public ::testing::Test
{
protected:

  virtual void SetUp()
  {
    plist1.push_back( pos{1,2,3} );
    plist1.push_back( pos{1,2,0,0} );
    plist1.push_back( pos{-1,0,1} );

    plist2.push_back( pos{1} );
  };

  std::list<Position::pos> plist1;
  std::list<Position::pos> plist2;
};

}; // Namespace Position

#endif /* TEST_POSITION_LIST_HPP */
