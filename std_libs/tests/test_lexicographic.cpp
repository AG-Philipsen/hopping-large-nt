/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 13:14:54 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"../lexicographic.hpp"
#include<gtest/gtest.h>
#include"utility_functions.hpp"

#include<vector>
#include<list>

using namespace Utility::Permutation;

template <
  typename Container,
  typename Compare = std::less<typename Container::iterator::value_type> >
Lexicographic<Container, Compare> CreateLexicographic(Container & c, Compare comp = Compare())
{
  return {std::begin(c), std::end(c), comp};
}

TEST(LexicographicTest, VectorConstructor)
{
  auto v = std::vector<int>{1,2};
  Lexicographic< std::vector<int> >{std::begin(v), std::end(v)};
  
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(2L,v.size());
  EXPECT_EQ(1,v[0]);
  EXPECT_EQ(2,v[1]);
}

TEST(LexicographicTest, ListConstructor)
{
  std::list<int> l{1,2};
  Lexicographic< std::list<int> >{std::begin(l), std::end(l)};

  EXPECT_FALSE(l.empty());
  EXPECT_EQ(2L, l.size());
  EXPECT_EQ(1, l.front());
  EXPECT_EQ(2, l.back());
}

TEST(LexicographicTest, FalseOperator)
{
  auto v = std::vector<int>{1,2};
  auto lex = CreateLexicographic(v);

  EXPECT_TRUE(!lex);
  EXPECT_FALSE(static_cast<bool>(lex));
}

TEST(LexicographicTest, EmptyContainer)
{
  auto v = std::vector<int>{};
  auto lex = CreateLexicographic(v);

  EXPECT_FALSE(!lex);
  EXPECT_TRUE(static_cast<bool>(lex));

  lex.operator++();

  EXPECT_FALSE(!lex);
  EXPECT_TRUE(static_cast<bool>(lex));
}

TEST(LexicographicTest, SingleElementContainer)
{
  auto v = std::vector<int>{1};
  auto lex = CreateLexicographic(v);

  EXPECT_TRUE(!lex);
  EXPECT_FALSE(static_cast<bool>(lex));

  lex.operator++();

  EXPECT_FALSE(!lex);
  EXPECT_TRUE(static_cast<bool>(lex));
}

TEST(LexicographicTest, IncrementOperator)
{
  auto v = std::vector<int>{1,2};
  auto lex = CreateLexicographic(v);

  lex.operator++();

  ASSERT_EQ(2L, v.size());

  auto v_result = std::vector<int>{2,1};
  EXPECT_EQ(v_result, v);

  EXPECT_TRUE(!lex);
  EXPECT_FALSE(static_cast<bool>(lex));

  lex.operator++();

  EXPECT_EQ(v_result, v);

  EXPECT_FALSE(!lex);
  EXPECT_TRUE(static_cast<bool>(lex));
}

TEST(LexicographicTest, OverIncrementation)
{
  auto v = std::vector<int>{1,2};
  auto lex = CreateLexicographic(v);

  lex.operator++();
  lex.operator++();

  for(int i = 0; i < 5; ++i) {
    ++lex;
    ASSERT_FALSE(!lex);
    ASSERT_TRUE(static_cast<bool>(lex));
  }
}

TEST(LexicographicTest, CustomComparator)
{
  auto v = std::vector<int>{2,1};
  auto comp = std::greater<int>{};
  auto lex = CreateLexicographic(v, comp);

  lex.operator++();

  auto v_expected = std::vector<int>{1,2};

  ASSERT_EQ(v_expected, v);

  EXPECT_TRUE(!lex);
  EXPECT_FALSE(static_cast<bool>(lex));

  lex.operator++();

  EXPECT_EQ(v_expected, v);

  EXPECT_FALSE(!lex);
  EXPECT_TRUE(static_cast<bool>(lex));
}

TEST(LexicographicTest, RepeatingElements)
{
  auto v = std::vector<int>{1,2,2};
  auto lex = CreateLexicographic(v);

  auto expected = std::vector< std::vector<int> > {
    {1,2,2}, {2,1,2}, {2,2,1} };

  for(int i = 0; !lex; ++lex, ++i)
    ASSERT_EQ(expected[i], v);
}

TEST(LexicographicTest, LongRun)
{
  auto lex_result = UnitTest::ParsePermData("tests/Data/lex");

  auto l = lex_result.front();
  auto lex = CreateLexicographic(l);

  auto it = std::begin(lex_result);
  
  for(; !lex; ++lex) {
    ASSERT_NE(std::end(lex_result), it);
    ASSERT_EQ(*it, l);
    ++it;
  }
}
