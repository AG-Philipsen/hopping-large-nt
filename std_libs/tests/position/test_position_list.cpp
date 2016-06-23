/*
 * Created: 10-10-2014
 * Modified: Tue 14 Oct 2014 12:13:59 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"test_position_list.hpp"
#include"../utility_functions.hpp"

namespace Position {

TEST_F(PositionListTest, Constructor)
{
  List l {plist1};

  EXPECT_FALSE(l.empty());
  EXPECT_EQ(plist1.size(), l.size());
}

TEST_F(PositionListTest, Iterator)
{
  List l {plist1};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, plist1, l);
}

TEST_F(PositionListTest, PushBack)
{
  auto initial_size = plist1.size();

  auto list_copy = plist1;
  List l {list_copy};

  l.push_back(pos{1});

  EXPECT_EQ(initial_size + 1, l.size());
  EXPECT_EQ(initial_size + 1, list_copy.size());

  auto list_copy_2 = list_copy;

  list_copy.pop_back();
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, plist1, l);

  list_copy.push_back(pos{1});
  EXPECT_EQ(list_copy_2, list_copy);
}

TEST_F(PositionListTest, PushBackIterator)
{
  List l_init {plist1};
  auto initial_size = plist1.size();

  auto list_copy = plist1;
  List l_copy {list_copy};

  l_copy.push_back_iterator(l_init.begin());

  EXPECT_EQ(initial_size + 1, l_copy.size());
  EXPECT_EQ(initial_size + 1, list_copy.size());

  list_copy.pop_back();
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, plist1, l_copy);
}

TEST_F(PositionListTest, Clone)
{
  auto initial_size = plist1.size();
  auto list_copy = plist1;

  List l1 {list_copy};
  auto pl1 = l1.clone();

  pl1->push_back(pos{1});

  EXPECT_EQ(initial_size + 1, list_copy.size());
  EXPECT_EQ(initial_size + 1, l1.size());
  EXPECT_EQ(initial_size + 1, pl1->size());

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, l1, *pl1);

  list_copy.pop_back();
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, plist1, *pl1);
}

TEST_F(PositionListTest, MergeIterator)
{
  List l1 {plist1};
  List l2 {plist1};

  ASSERT_ANY_THROW(l1.merge_iterator(l2.begin(), l2.end()));
}

}; // Namespace Position
