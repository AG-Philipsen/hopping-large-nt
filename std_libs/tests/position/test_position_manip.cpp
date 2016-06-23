/*
 * Created: 14-10-2014
 * Modified: Wed 15 Oct 2014 13:20:23 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<position/position_manip.hpp>
#include<position/position_list.hpp>
#include<gtest/gtest.h>
#include"../utility_functions.hpp"
#include"test_commands.hpp"

#include<list>

namespace Position {

TEST(CoordinateCleanerTest, SameLength)
{
  auto l = std::list<pos> {
    {0,0,1},
    {1,0,0},
    {1,0,1} };

  auto cleaner = Manipulator::CoordinateCleaner{};
  cleaner.execute(l.begin(), l.end());

  auto expected = std::list<pos> {
    {0,1},
    {1,0},
    {1,1} };

  EXPECT_EQ(expected, l);
}

TEST(CoordinateCleanerTest, DifferentLength)
{
  auto l = std::list<pos> {
    {0,0,1},
    {1,0,1,0},
    {1},
    {1,0,1} };

  auto cleaner = Manipulator::CoordinateCleaner{};
  cleaner.execute(l.begin(), l.end());

  auto expected = std::list<pos> {
    {0,1},
    {1,1},
    {1,0},
    {1,1} };

  EXPECT_EQ(expected, l);
}

TEST(CoordinateCleanerTest, LongerRun)
{
  auto pos_list = std::vector<pos> {
    {1,0,8,0,0,1,0,0,4},
    {4},
    {0,0,0,0,0,3,0,0,0,0,0,1},
    {-31,0,14,7,0,1,0,0,4,9,1,0,1},
    {0,0,0,0,0,0,0},
    {4,0,0,9,0,7}
  };

  auto cleaner = Manipulator::CoordinateCleaner{};
  cleaner.execute(pos_list.begin(), pos_list.end());

  auto expected = std::vector<pos> {
    {  1, 8,0,1,4,0,0,0,0},
    {  4, 0,0,0,0,0,0,0,0},
    {  0, 0,0,3,0,0,0,1,0},
    {-31,14,7,1,4,9,1,0,1},
    {  0, 0,0,0,0,0,0,0,0},
    {  4, 0,9,7,0,0,0,0,0}
  };

  ASSERT_EQ(expected.size(), pos_list.size());

  for(std::size_t index = 0; index < expected.size(); ++index)
  {
    EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected.at(index), pos_list.at(index));
  }
}

TEST(ForAllPositionsTest, ShortRun)
{
  auto pos_list = std::list<pos> {
    {0}, {1}, {2} };

  auto expected_pos_list = std::list< std::list<pos> > {
    { { 0}, { 1}, {2} },
    { {-1}, { 0}, {1} },
    { {-2}, {-1}, {0} } };

  List list_proxy {pos_list};
  auto command = Command::TestRelPosCommand {pos_list, expected_pos_list};

  auto for_all_macro = Manipulator::ForAllPositions {&command};
  for_all_macro.ForEach(list_proxy.begin(), list_proxy.end());
}

TEST(ForAllPositionsTest, LongerRun)
{
  auto pos_list = std::list<pos> {
    {1,-2,4}, {1,2,0}, {0,4,1}, {-1,0,-1} };

  auto expected_pos_list = std::list< std::list<pos> > {
    { { 0, 0, 0}, { 0, 4,-4}, {-1, 6,-3}, {-2, 2,-5} },
    { { 0,-4, 4}, { 0, 0, 0}, {-1, 2, 1}, {-2,-2,-1} },
    { { 1,-6, 3}, { 1,-2,-1}, { 0, 0, 0}, {-1,-4,-2} },
    { { 2,-2, 5}, { 2, 2, 1}, { 1, 4, 2}, { 0, 0, 0} } };

  List list_proxy {pos_list};
  auto command = Command::TestRelPosCommand {pos_list, expected_pos_list};

  auto for_all_macro = Manipulator::ForAllPositions {&command};
  for_all_macro.ForEach(list_proxy.begin(), list_proxy.end());
}

TEST(RelativePositionSummerTest, OneTermOneDim)
{
  auto positions = std::list<pos> { {1} };
  List list_ref {positions};

  auto command = Command::TestSimpleTermCollector<1>{};
  auto summer = Manipulator::RelPosSummer<1>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected_result = Command::TestSimpleTermCollector<1>::container_type {
    { {{1}} } };

  EXPECT_EQ(expected_result, result);
}

TEST(RelativePositionSummerTest, OneTermThreeDims)
{
  auto positions = std::list<pos> { {1} };
  List list_ref {positions};

  auto command = Command::TestSimpleTermCollector<3>{};
  auto summer = Manipulator::RelPosSummer<3>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected_result = Command::TestSimpleTermCollector<3>::container_type {
    { {{1,0,0}} }, { {{0,1,0}} }, { {{0,0,1}} } };

  EXPECT_EQ(expected_result, result);
}

TEST(RelativePositionSummerTest, TwoTermsOneDim)
{
  auto positions = std::list<pos> { {1}, {2} };
  List list_ref {positions};

  auto command = Command::TestSimpleTermCollector<1>{};
  auto summer = Manipulator::RelPosSummer<1>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected_result = Command::TestSimpleTermCollector<1>::container_type {
    { {{1}}, {{2}} } };

  EXPECT_EQ(expected_result, result);
}

TEST(RelativePositionSummerTest, TwoTermsThreeDims)
{
  auto positions = std::list<pos> { {1}, {2} };
  List list_ref {positions};

  auto command = Command::TestSimpleTermCollector<3>{};
  auto summer = Manipulator::RelPosSummer<3>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected_result = Command::TestSimpleTermCollector<3>::container_type {
    { {{1,0,0}}, {{2,0,0}} }, { {{0,1,0}}, {{0,2,0}} }, { {{0,0,1}}, {{0,0,2}} } };

  EXPECT_EQ(expected_result, result);
}

TEST(RelativePositionSummerTest, TwoDirectionsOneDim)
{
  auto positions = std::list<pos> { {1,1} };
  List list_ref {positions};

  auto command = Command::TestAdvancedTermCollector<1>{};
  auto summer = Manipulator::RelPosSummer<1>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected = Command::TestAdvancedTermCollector<1>::container_type{};
  expected.insert( { { {{2}} }, 1} );

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, result);
}

TEST(RelativePositionSummerTest, TwoDirectionsTwoDims)
{
  auto positions = std::list<pos> { {1,1} };
  List list_ref {positions};

  auto command = Command::TestAdvancedTermCollector<2>{};
  auto summer = Manipulator::RelPosSummer<2>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected = Command::TestAdvancedTermCollector<2>::container_type{};
  expected.insert( { { {{2,0}} }, 1} );
  expected.insert( { { {{0,2}} }, 1} );
  expected.insert( { { {{1,1}} }, 2} );

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, result);
}

TEST(RelativePositionSummerTest, TwoDirectionsThreeDims)
{
  auto positions = std::list<pos> { {1,1} };
  List list_ref {positions};

  auto command = Command::TestAdvancedTermCollector<3>{};
  auto summer = Manipulator::RelPosSummer<3>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected = Command::TestAdvancedTermCollector<3>::container_type{};
  expected.insert( { { {{2,0,0}} }, 1} );
  expected.insert( { { {{0,2,0}} }, 1} );
  expected.insert( { { {{0,0,2}} }, 1} );
  expected.insert( { { {{1,1,0}} }, 2} );
  expected.insert( { { {{1,0,1}} }, 2} );
  expected.insert( { { {{0,1,1}} }, 2} );

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, result);
}

TEST(RelativePositionSummerTest, ZeroPosition)
{
  auto positions = std::list<pos> { {1,1,0,0} };
  List list_ref {positions};

  auto command = Command::TestAdvancedTermCollector<3>{};
  auto summer = Manipulator::RelPosSummer<3>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected = Command::TestAdvancedTermCollector<3>::container_type{};
  expected.insert( { { {{2,0,0}} },  9} );
  expected.insert( { { {{0,2,0}} },  9} );
  expected.insert( { { {{0,0,2}} },  9} );
  expected.insert( { { {{1,1,0}} }, 18} );
  expected.insert( { { {{1,0,1}} }, 18} );
  expected.insert( { { {{0,1,1}} }, 18} );

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, result);
}

TEST(RelativePositionSummerTest, MultiDirDim)
{
  auto positions = std::list<pos> { {1,0,0,1}, {0,0,1,1} };
  List list_ref {positions};

  auto command = Command::TestAdvancedTermCollector<3>{};
  auto summer = Manipulator::RelPosSummer<3>{&command};

  summer.ForAll(list_ref.begin(), list_ref.end());
  auto result = command.ExtractData();

  auto expected = Command::TestAdvancedTermCollector<3>::container_type{};
  expected.insert( { { {{2,0,0}}, {{2,0,0}} }, 3} );
  expected.insert( { { {{2,0,0}}, {{1,1,0}} }, 3} );
  expected.insert( { { {{2,0,0}}, {{1,0,1}} }, 3} );

  expected.insert( { { {{1,1,0}}, {{1,1,0}} }, 6} );
  expected.insert( { { {{1,1,0}}, {{0,2,0}} }, 3} );
  expected.insert( { { {{1,1,0}}, {{0,1,1}} }, 3} );
  expected.insert( { { {{1,1,0}}, {{1,0,1}} }, 3} );
  expected.insert( { { {{1,1,0}}, {{2,0,0}} }, 3} );

  expected.insert( { { {{0,2,0}}, {{1,1,0}} }, 3} );
  expected.insert( { { {{0,2,0}}, {{0,2,0}} }, 3} );
  expected.insert( { { {{0,2,0}}, {{0,1,1}} }, 3} );

  expected.insert( { { {{1,0,1}}, {{1,0,1}} }, 6} );
  expected.insert( { { {{1,0,1}}, {{0,1,1}} }, 3} );
  expected.insert( { { {{1,0,1}}, {{0,0,2}} }, 3} );
  expected.insert( { { {{1,0,1}}, {{2,0,0}} }, 3} );
  expected.insert( { { {{1,0,1}}, {{1,1,0}} }, 3} );

  expected.insert( { { {{0,0,2}}, {{1,0,1}} }, 3} );
  expected.insert( { { {{0,0,2}}, {{0,1,1}} }, 3} );
  expected.insert( { { {{0,0,2}}, {{0,0,2}} }, 3} );

  expected.insert( { { {{0,1,1}}, {{1,1,0}} }, 3} );
  expected.insert( { { {{0,1,1}}, {{0,2,0}} }, 3} );
  expected.insert( { { {{0,1,1}}, {{0,1,1}} }, 6} );
  expected.insert( { { {{0,1,1}}, {{1,0,1}} }, 3} );
  expected.insert( { { {{0,1,1}}, {{0,0,2}} }, 3} );

  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, expected, result);
}

} //Namespace Position
