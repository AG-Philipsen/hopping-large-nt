/*
 * Created: 14-10-2014
 * Modified: Wed 15 Oct 2014 10:41:00 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"test_commands.hpp"
#include<gtest/gtest.h>
#include"../utility_functions.hpp"

namespace Position {
namespace Command {

void TestRelPosCommand::Execute(container::iterator it)
{
  ASSERT_EQ(base_list.size(), result_pos->size());

  auto result_it = result_pos->begin();
  auto base_it = base_list.begin();

  while(result_it != result_pos->end())
  {
    EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, *result_it, *base_it);

    ++result_it;
    ++base_it;
  }

  ++result_pos;
}

}}
