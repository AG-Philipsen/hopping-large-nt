/*
 * Created: 02-10-2014
 * Modified: Wed 29 Oct 2014 10:27:39 CET
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef UTILITY_FUNCTIONS_HPP
#define UTILITY_FUNCTIONS_HPP

#include<list>
#include<vector>
#include<string>
#include<sstream>
#include<gtest/gtest.h>
#include"prettyprint.hpp"

namespace UnitTest {

std::list< std::vector<int> > ParsePermData(std::string filename);

template <typename InputIt>
std::string IteratorToString(InputIt begin, InputIt end)
{
  if(begin == end)
    return "[]";

  std::stringstream ss;

  ss << "[" << *begin;
  ++begin;

  while(begin != end)
  {
    ss << ", " << *begin;
    ++begin;
  }

  ss << "]";

  return ss.str();
}

template <typename Container1, typename Container2>
inline ::testing::AssertionResult 
ContainerCompare(const char* expect_expr,
                 const char* actual_expr,
                 const Container1 & expected,
                 const Container2 & actual)
{
  using std::begin;
  using std::end;

  if(std::equal(begin(expected), end(expected), begin(actual)))
    return ::testing::AssertionSuccess();

  return ::testing::AssertionFailure()
    << "Value of: " << actual_expr << "\n"
    << "  Actual: " << actual << "\n"
    << "Expected: " << expect_expr << "\n"
    << "Which is: " << expected;
}

}

#endif /* UTILITY_FUNCTIONS_HPP */
