/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 16:12:01 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"../sjt_permutations.hpp"
#include<gtest/gtest.h>
#include"utility_functions.hpp"

#include<boost/lexical_cast.hpp>

using namespace Utility::Permutation;

void SjtRuns(std::size_t number)
{
  using std::begin;
  using std::end;

  std::string filename = "tests/Data/sjt" + boost::lexical_cast<std::string>(number);
  auto result = UnitTest::ParsePermData(filename);

  auto sjt = SJT{number,1};

  for(auto it = std::begin(result); !sjt ; ++sjt, ++it)
  {
    ASSERT_NE(std::end(result), it);
    EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, *it, sjt);
  }
}

TEST(SJTTest, DefaultConstructor)
{
  auto sjt = SJT{};

  EXPECT_FALSE(!sjt);
  EXPECT_TRUE(static_cast<bool>(sjt));
}

TEST(SJTTest, SingleElement)
{
  auto sjt = SJT{1};

  EXPECT_TRUE(!sjt);
  EXPECT_FALSE(static_cast<bool>(sjt));

  ++sjt;

  EXPECT_FALSE(!sjt);
  EXPECT_TRUE(static_cast<bool>(sjt));
}

TEST(SJTTest, Iterator)
{
  using std::begin;
  using std::end;

  auto sjt = SJT{2};

  EXPECT_EQ(2L, std::distance(begin(sjt), end(sjt)));

  auto v1 = std::vector<int>{0,1};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v1, sjt);

  ++sjt;
  auto v2 = std::vector<int>{1,0};
  EXPECT_PRED_FORMAT2(UnitTest::ContainerCompare, v2, sjt);

  auto sjt_empty = SJT{};
  EXPECT_EQ(0L, std::distance(begin(sjt_empty), end(sjt_empty)));
}

TEST(SJTTest, AccessOperator)
{
  auto sjt = SJT{2};
  auto v = std::vector<int>{0,1};

  for(std::size_t i = 0; i < v.size(); ++i)
    EXPECT_EQ(v[i], sjt[i]);

  for(std::size_t i = 0; i < v.size(); ++i)
    EXPECT_EQ(v[i], sjt.at(i));

  ++sjt;
  std::swap(v[0],v[1]);

  for(std::size_t i = 0; i < v.size(); ++i)
    EXPECT_EQ(v[i], sjt[i]);

  for(std::size_t i = 0; i < v.size(); ++i)
    EXPECT_EQ(v[i], sjt.at(i));
}

TEST(SJTTest, AtOutOfBounds)
{
  auto sjt = SJT{1};

  ASSERT_THROW(sjt.at(1),std::out_of_range);
}

TEST(SJTTest, CustomStart)
{
  using std::begin;
  using std::end;

  auto sjt = SJT{2,2};
  auto v = std::vector<int>{2,3};

  ASSERT_TRUE(std::equal(begin(v), end(v), begin(sjt)));

  ++sjt;
  std::swap(v[0],v[1]);

  EXPECT_TRUE(std::equal(begin(v), end(v), begin(sjt)));
}

TEST(SJTTest, Run3)
{
  SjtRuns(3);
}

TEST(SJTTest, Run5)
{
  SjtRuns(5);
}

TEST(SJTTest, Run8)
{
  SjtRuns(8);
}

TEST(SJTTest, DISABLED_Run10)
{
  SjtRuns(10);
}
