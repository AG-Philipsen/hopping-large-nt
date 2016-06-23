//Created: 29-09-2014
//Modified: Wed 25 Mar 2015 12:15:13 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef TEST_MATH_TERM_HPP
#define TEST_MATH_TERM_HPP

#include"../math_term.hpp"
#include"../math_sum.hpp"
#include<gtest/gtest.h>
#include<string>

class AbelianSumTest : public ::testing::Test
{
protected:
  virtual void SetUp();

  AbelianSumTest();

  std::string sa;
  std::string sb;

  Math::Term<std::string> ta;
  Math::Term<std::string> tb;
  Math::Term<std::string> tc;

  Math::Sum<std::string> suma;
  Math::Sum<std::string> sumb;
  Math::Sum<std::string> sumc;
};

class NonAbelianSumTest : public ::testing::Test
{
protected:
  virtual void SetUp();

  NonAbelianSumTest();

  std::string sa;
  std::string sb;

  Math::NonAbelianTerm<std::string> ta;
  Math::NonAbelianTerm<std::string> tb;
  Math::NonAbelianTerm<std::string> tc;

  Math::NonAbelianSum<std::string> suma;
  Math::NonAbelianSum<std::string> sumb;
};


#endif /* TEST_MATH_TERM_HPP */
