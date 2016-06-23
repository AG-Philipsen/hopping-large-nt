/*
 * Created: 25-02-2015
 * Modified: Wed 25 Feb 2015 14:02:49 CET
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<gtest/gtest.h>
#include<position/position_relabel.hpp>
#include"../prettyprint.hpp"

namespace Position {
namespace Manipulator {

typedef PositionRelabeler::relabel_type relabel_type;
typedef PositionRelabeler::relabel_container relabel_container;

TEST(PositionRelabelerTest, SimpleFlip)
{
  auto p = pos {0,1};
  auto relabel = relabel_type {0,1};

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {1,0};
  EXPECT_EQ(expected, p);
}

TEST(PositionRelabelerTest, OutOfBoundsFlip)
{
  auto p = pos {1};
  auto relabel = relabel_type {0,4};

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {0,0,0,0,1};
  EXPECT_EQ(expected, p);
}

TEST(PositionRelabelerTest, PersistencyCheck)
{
  auto p = pos {1,2,3,4,5};
  auto relabel = relabel_type {1,3};

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {1,4,3,2,5};
  EXPECT_EQ(expected, p);
}

TEST(PositionRelabelerTest, IndependentRelabelings)
{
  auto p = pos {1,2,3,4};
  auto relabel = relabel_container {
    {0,1},
    {2,3} };

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {2,1,4,3};
  EXPECT_EQ(expected, p);
}

TEST(PositionRelabelerTest, ChainRelabelings)
{
  auto p = pos {1,2,3};
  auto relabel = relabel_container {
    {0,1},
    {1,2} };

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {2,3,1};
  EXPECT_EQ(expected, p);
}

TEST(PositionRelabelerTest, LongerChainRelabelings)
{
  auto p = pos {1,2,3};
  auto relabel = relabel_container {
    {0,1},
    {1,2},
    {2,0},
    {0,1} };

  auto relabeler = PositionRelabeler {};
  relabeler(p, relabel);

  auto expected = pos {3,1,2};
  EXPECT_EQ(expected, p);
}

TEST(PositionContainerRelabelerTest, SinglePosition)
{
  auto positions = std::vector<pos> {
    {0,1} };

  auto relabeler = PositionContainerRelabeler {};
  relabeler(positions.begin(), positions.end());

  auto expected_positions = std::vector<pos> {
    {1,0} };

  EXPECT_EQ(expected_positions, positions);
}

TEST(PositionContainerRelabelerTest, TwoIdentital)
{
  auto positions = std::vector<pos> {
    {0,1},
    {0,1} };

  auto relabeler = PositionContainerRelabeler {};
  relabeler(positions.begin(), positions.end());

  auto expected_positions = std::vector<pos> {
    {1,0},
    {1,0} };

  EXPECT_EQ(expected_positions, positions);
}

TEST(PositionContainerRelabelerTest, TwoSeparate)
{
  auto positions = std::vector<pos> {
    {0,1,0,0},
    {1,0,0,2} };

  auto relabeler = PositionContainerRelabeler {};
  relabeler(positions.begin(), positions.end());

  auto expected_positions = std::vector<pos> {
    {1,0,0,0},
    {0,1,2,0} };

  EXPECT_EQ(expected_positions, positions);
}

TEST(PositionContainerRelabelerTest, TwoDependent)
{
  auto positions = std::vector<pos> {
    {0,1,0,2},
    {1,0,0,2} };

  auto relabeler = PositionContainerRelabeler {};
  relabeler(positions.begin(), positions.end());

  auto expected_positions = std::vector<pos> {
    {1,2,0,0},
    {0,2,1,0} };

  EXPECT_EQ(expected_positions, positions);
}

TEST(PositionContainerRelabelerTest, SimpleWithLocked)
{
  auto positions = std::vector<pos> {
    {0,0,1,0,2,3} };

  auto locked = std::set<pos::size_type> {0,4};

  auto relabeler = PositionContainerRelabeler {};
  relabeler(positions.begin(), positions.end(), locked);

  auto expected_positions = std::vector<pos> {
    {0,1,3,0,2,0} };

  EXPECT_EQ(expected_positions, positions);
}

} //Namespace Manipulator
} //Namespace Position
