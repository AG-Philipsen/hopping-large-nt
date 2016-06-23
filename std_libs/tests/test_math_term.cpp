//Created: 29-09-2014
//Modified: Fri 17 Oct 2014 19:54:17 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"test_math_term.hpp"

TEST(TermTest, DefaultConstructor)
{
  auto t = Math::Term<char>{};

  EXPECT_EQ(1L, t.prefactor());
  EXPECT_TRUE(t.empty());
}

TEST(TermTest, Constructors)
{
  auto ta = Math::Term<char>{'a'};
  EXPECT_EQ(1L, ta.prefactor());

  auto it = std::begin(ta);
  EXPECT_EQ('a', *it);

  auto tb = Math::Term<std::string>{100L};
  EXPECT_EQ(100L, tb.prefactor());
  EXPECT_TRUE(tb.empty());
}

TEST(TermTest, CopyConstructor)
{
  using std::string;

  auto s = std::string{ "a" };

  auto ta = Math::Term<string>{ s };
  ta.prefactor_ref() = 24L;

  auto tb = ta;

  EXPECT_EQ(1L, tb.size());
  EXPECT_EQ(s, *(tb.begin()));
}

TEST(TermTest, PrefactorRef)
{
  auto t = Math::Term<char>{'a'};
  t.prefactor_ref() *= 24;

  EXPECT_EQ(24,t.prefactor());
}

TEST(TermTest, MultiplyEqualNumeric)
{
  using std::string;

  auto s = string{ "a" };
  auto t = Math::Term<string>{ s };

  t *= 24L;

  EXPECT_EQ(1L, t.size());
  EXPECT_EQ(24L, t.prefactor());
}

TEST(TermTest, MultiplyEqualType)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::Term<string>{ sa };

  t *= sb;

  EXPECT_EQ(2L, t.size());
  EXPECT_EQ(1L, t.prefactor());

  auto it = t.begin();
  EXPECT_EQ(sa, *it);

  ++it;
  EXPECT_EQ(sb, *it);
}

TEST(TermTest, MultiplyEqualTerm)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto ta = Math::Term<string>{ sa };
  auto tb = Math::Term<string>{ sb };

  ta.prefactor_ref() = 3L;
  tb.prefactor_ref() = 24L;

  tb *= sa;

  ta *= tb;

  EXPECT_EQ(3L, ta.size());
  EXPECT_EQ(72L, ta.prefactor());

  auto it = ta.begin();
  EXPECT_EQ(sa, *it);

  ++it;
  EXPECT_EQ(sb, *it);

  ++it;
  EXPECT_EQ(sa, *it);
}

TEST(TermTest, MultiplicationTerms)
{
  using std::string;

  string sa { "a" };
  string sb { "b" };

  auto ta = Math::Term<string>{ sa };
  auto tb = Math::Term<string>{ sb };

  auto tc = ta * tb;

  EXPECT_EQ(2L, tc.size());
  EXPECT_EQ(1L, tc.prefactor());

  auto it = tc.begin();
  EXPECT_EQ(sa, *it);

  ++it;
  EXPECT_EQ(sb, *it);

  ++it;
  EXPECT_EQ(std::end(tc), it);
}

TEST(TermTest, MultiplicationType)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::Term<string>{ sa };
  auto tra = t * sb;

  EXPECT_EQ(2L, tra.size());
  EXPECT_EQ(1L, tra.prefactor());

  EXPECT_EQ(sa, *(tra.begin()));

  auto trb = sb * t;

  EXPECT_EQ(2L, trb.size());
  EXPECT_EQ(1L, trb.prefactor());

  EXPECT_EQ(sb, *(trb.begin()));
}

TEST(TermTest, MultiplicationPrefactor)
{
  auto t = Math::Term<std::string>{};

  auto tra = t * 100L;
  EXPECT_TRUE(tra.empty());
  EXPECT_EQ(100L, tra.prefactor());

  auto trb = 100L * t;
  EXPECT_TRUE(trb.empty());
  EXPECT_EQ(100L, trb.prefactor());
}

TEST(TermAbilianTest, Sort)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::Term<string>{ sb };
  t *= sa;
  t *= sa;
  t *= 24L;

  EXPECT_NO_THROW(t.sort());

  EXPECT_EQ(24L, t.prefactor());
  EXPECT_EQ(3L, t.size());

  auto expected = std::vector<string>{ sa, sa, sb };

  auto expected_it = std::begin(expected);
  auto t_it = std::begin(t);

  while( expected_it != std::end(expected) and t_it != std::end(t))
  {
    EXPECT_EQ(*expected_it, *t_it);
    ++expected_it; ++t_it;
  }
}

TEST(TermNonAbelian, Sort)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::Term<string,long long,std::less<string>,false>{sa};
  t *= sb;

  EXPECT_THROW(t.sort(), std::logic_error);
}

TEST(TermTest, Comparison)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto ta = Math::Term<string>{ sa };
  auto tb = Math::Term<string>{ sb };

  EXPECT_TRUE(ta < tb);
  EXPECT_FALSE(tb < ta);

  ta *= 200L;

  EXPECT_TRUE(ta < tb);
  EXPECT_FALSE(tb < ta);
}

AbelianSumTest::AbelianSumTest()
  : sa{"a"},
    sb{"b"},
    ta{sa},
    tb{sb},
    tc{sb}
{};


void AbelianSumTest::SetUp()
{
  { //Setting up suma
    ta *= sb;
    tb *= sb;
    tc *= sa;

    suma += ta;
    suma += tb;

    ta *= 23L;

    suma += ta;
    suma += tc;
  }

  { //Setting up sumb
    tb *= 22L;
    sumb += tb;

    tc *= 10L;
    sumb += tc;
  }

  { //Setting up sumc
    sumc -= ta;
    sumc -= tb;
    sumc += tc;
  }
}

TEST(SumTest, DefaultConstructor)
{
  auto s = Math::Sum<int>{};

  ASSERT_TRUE(s.empty());
}

TEST_F(AbelianSumTest, AddingTerm)
{
  EXPECT_EQ(2L, suma.size());

  auto it = suma.begin();

  EXPECT_EQ(25L, it->prefactor());

  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;
  EXPECT_EQ(1L, it->prefactor());

  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

TEST_F(AbelianSumTest, SubtractingTerm)
{
  EXPECT_EQ(2L, sumc.size());

  auto it = sumc.begin();

  EXPECT_EQ(-13L, it->prefactor());

  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;

  EXPECT_EQ(-22L, it->prefactor());

  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

TEST_F(AbelianSumTest, CopyConstructor)
{
  auto sumc = suma;

  EXPECT_EQ(2L, sumc.size());

  auto it = sumc.begin();

  EXPECT_EQ(25L, it->prefactor());

  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;
  EXPECT_EQ(1L, it->prefactor());

  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

TEST_F(AbelianSumTest, AddingSum)
{
  auto sumc = suma;

  sumc += sumb;

  EXPECT_EQ(2L, sumc.size());

  auto it = sumc.begin();

  EXPECT_EQ(35L, it->prefactor());

  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;
  EXPECT_EQ(23L, it->prefactor());

  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

NonAbelianSumTest::NonAbelianSumTest()
  : sa{"a"},
    sb{"b"},
    ta{sa},
    tb{sb},
    tc{sb}
{};

void NonAbelianSumTest::SetUp()
{
  { //Setting up suma
    ta *= sb;
    tb *= sb;
    tc *= sa;

    tc *= 10L;

    suma += ta;
    suma += tb;

    ta *= 23L;

    suma += ta;
    suma += tc;
  }

  { //Setting up sumb
    tb *= 22L;
    sumb += tb;
    sumb += tc;
  }
}

TEST_F(NonAbelianSumTest, AddingTerm)
{
  EXPECT_EQ(3L, suma.size());

  auto it = suma.begin();

  EXPECT_EQ(24L, it->prefactor());
  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;
  
  EXPECT_EQ(10L, it->prefactor());
  EXPECT_FALSE(tc < *it);
  EXPECT_FALSE(*it < tc);

  ++it;
  
  EXPECT_EQ(1L, it->prefactor());
  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

TEST_F(NonAbelianSumTest, AddingSum)
{
  auto sumc = suma;
  sumc += sumb;

  EXPECT_EQ(3L, sumc.size());

  auto it = sumc.begin();

  EXPECT_EQ(24L, it->prefactor());
  EXPECT_FALSE(ta < *it);
  EXPECT_FALSE(*it < ta);

  ++it;
  
  EXPECT_EQ(20L, it->prefactor());
  EXPECT_FALSE(tc < *it);
  EXPECT_FALSE(*it < tc);

  ++it;
  
  EXPECT_EQ(23L, it->prefactor());
  EXPECT_FALSE(tb < *it);
  EXPECT_FALSE(*it < tb);
}

TEST(SortTermTest, AbelianSort)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::Term<string>{ sb };

  t *= sa;
  t *= sb;

  SortTerm(t);

  EXPECT_EQ(3L, t.size());

  auto it = t.begin();
  EXPECT_EQ(sa, *it);
  
  ++it;
  EXPECT_EQ(sb, *it);

  ++it;
  EXPECT_EQ(sb, *it);
}

TEST(SortTermTest, NonAbelianSort)
{
  using std::string;

  auto sa = string{ "a" };
  auto sb = string{ "b" };

  auto t = Math::NonAbelianTerm<string>{ sb };

  t *= sa;
  t *= sb;

  SortTerm(t);

  EXPECT_EQ(3L, t.size());

  auto it = t.begin();
  EXPECT_EQ(sb, *it);
  
  ++it;
  EXPECT_EQ(sa, *it);

  ++it;
  EXPECT_EQ(sb, *it);
}
