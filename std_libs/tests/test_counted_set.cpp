//Created: 29-09-2014
//Modified: Wed 15 Oct 2014 15:34:25 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"../counted_set.hpp"
#include<gtest/gtest.h>

Utility::CountedSet<int> StandardSetup()
{
  auto s = Utility::CountedSet<int>{};

  s.insert(1);
  s.insert(4);
  s.insert(1);

  for(int i = 0; i < 8; ++i)
    s.insert(256);

  return s;
}

TEST(CountedSetTest, DefaultConstructor)
{
  Utility::CountedSet<int> s{};

  EXPECT_EQ(0L, s.size());
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.begin(), s.end());
}

TEST(CountedSetTest, InsertElements)
{
  auto s = StandardSetup();

  EXPECT_EQ(3L, s.size());
  EXPECT_NE(s.begin(), s.end());

  auto it = s.begin();

  EXPECT_EQ(1, it->obj());
  EXPECT_EQ(2, it->count());
  
  ++it;

  EXPECT_EQ(4, it->obj());
  EXPECT_EQ(1, it->count());

  ++it;

  EXPECT_EQ(256, it->obj());
  EXPECT_EQ(8, it->count());

  ++it;

  EXPECT_EQ(std::end(s), it);
}

TEST(CountedSetTest, Clear)
{
  auto s = StandardSetup();

  s.clear();

  EXPECT_EQ(0L, s.size());
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(std::begin(s), std::end(s));
}

TEST(CountedSetTest, Find)
{
  auto s = StandardSetup();

  auto find_four = s.find(4);

  EXPECT_EQ(4, find_four->obj());
  EXPECT_EQ(1, find_four->count());

  auto find_nine = s.find(9);

  EXPECT_EQ(std::end(s), find_nine);
}

TEST(CountedSetTest, InsertReturnValues)
{
  auto s = Utility::CountedSet<int>{};

  auto set_ret_1 = s.insert(4);
  auto it_1 = set_ret_1.first;

  EXPECT_NE(std::end(s), it_1);
  EXPECT_EQ(4, it_1->obj());
  EXPECT_EQ(1, it_1->count());
  EXPECT_TRUE(set_ret_1.second);

  auto set_ret_2 = s.insert(4);
  auto it_2 = set_ret_2.first;

  EXPECT_NE(std::end(s), it_2);
  EXPECT_EQ(4, it_2->obj());
  EXPECT_EQ(2, it_2->count());
  EXPECT_FALSE(set_ret_2.second);
}

TEST(CountedSetTest, InsertCounted)
{
  const int val = 5;

  auto c = Utility::Counted<int>{val, 4};
  auto s = Utility::CountedSet<int>{};
  
  s.insert(val);
  s.insert(c);

  EXPECT_EQ(1, s.size());

  auto it = s.begin();

  EXPECT_EQ(val, it->obj());
  EXPECT_EQ(5, it->count());
}

TEST(CountedSetTest, SetInsert)
{
  const int val = 5;
  auto s = Utility::CountedSet<int>{};

  s.insert(val);
  s.set_insert(val);

  EXPECT_EQ(1,s.size());

  auto it = std::begin(s);

  EXPECT_EQ(val, it->obj());
  EXPECT_EQ(1, it->count());
}

TEST(CountedSetTest, SetInsertCounted)
{
  const int val = 5;
  auto c = Utility::Counted<int>{ val };
  c.Increment(3);

  auto s = Utility::CountedSet<int>{};

  s.insert(val);
  s.set_insert(c);

  EXPECT_EQ(1, s.size());

  auto it = s.begin();

  EXPECT_EQ(val, it->obj());
  EXPECT_EQ(1, it->count());
}

TEST(CountedSetTest, RemoveOne)
{
  auto s = StandardSetup();

  auto rem_res_1 = s.remove_one(256);
  ASSERT_TRUE(rem_res_1.second);

  auto it_1 = s.find(256);
  ASSERT_NE(std::end(s), it_1);
  EXPECT_EQ(7, it_1->count());

  auto rem_res_2 = s.remove_one(4);
  ASSERT_TRUE(rem_res_2.second);

  auto it_2 = s.find(4);
  EXPECT_EQ(std::end(s), it_2);

  auto rem_res_3 = s.remove_one(100);
  EXPECT_FALSE(rem_res_3.second);
}

TEST(CountedSetTest, RemoveAll)
{
  auto s = StandardSetup();

  auto rem_res_t = s.remove_all(256);
  ASSERT_TRUE(rem_res_t.second);

  auto it = s.find(256);
  EXPECT_EQ(std::end(s), it);

  auto rem_res_f = s.remove_all(100);
  EXPECT_FALSE(rem_res_f.second);
}

TEST(CountedSetTest, EraseOne)
{
  auto s = StandardSetup();
  auto it = std::begin(s);

  it = s.erase_one(it);

  EXPECT_EQ(1, it->count());

  it = s.erase_one(it);
  
  EXPECT_EQ(2L, s.size());
  EXPECT_EQ(4, it->obj());

  it = std::end(s);

  ASSERT_THROW(s.erase_one(it), std::out_of_range);
}

TEST(CountedSetTest, EraseAll)
{
  auto s = StandardSetup();
  auto it = s.find(256);

  ASSERT_NE(std::end(s), it);

  it = s.erase_all(it);

  EXPECT_EQ(2L, s.size());
  EXPECT_EQ(std::end(s), it);

  it = std::end(s);

  ASSERT_THROW(s.erase_all(it), std::out_of_range);
}

TEST(CountedSetTest, Comparison)
{
  auto s1 = StandardSetup();
  auto s2 = StandardSetup();

  s1.insert(4);

  EXPECT_FALSE(s1 < s2);
  EXPECT_FALSE(s2 < s1);

  s1.insert(5);

  EXPECT_TRUE(s1 < s2);
}

TEST(CountedSetTest, ExtractData)
{
  auto s = StandardSetup();
  auto data = s.extract_data();

  EXPECT_TRUE(s.empty());

  ASSERT_FALSE(data.empty());
  EXPECT_EQ(3L, data.size());

  auto & last_elem = data[2];

  EXPECT_EQ(256, last_elem.obj());
  EXPECT_EQ(8, last_elem.count());
}

TEST(CountedSetFullCompareTest, EqualSets)
{
  auto s1 = StandardSetup();
  auto s2 = StandardSetup();

  auto comp = Utility::CountedSetFullCompare<int>{};

  EXPECT_FALSE(comp(s1,s2));
  EXPECT_FALSE(comp(s2,s1));
} 

TEST(CountedSetFullCompareTest, DifferentObjects)
{
  auto s1 = StandardSetup();
  auto s2 = StandardSetup();

  auto comp = Utility::CountedSetFullCompare<int>{};

  s1.insert(5);

  EXPECT_TRUE(comp(s1,s2));
  EXPECT_FALSE(comp(s2,s1));
}

TEST(CountedSetFullCompareTest, DifferentCount)
{
  auto s1 = StandardSetup();
  auto s2 = StandardSetup();

  auto comp = Utility::CountedSetFullCompare<int>{};

  auto it = s1.begin();
  it->Increment();

  EXPECT_FALSE(comp(s1,s2));
  EXPECT_TRUE(comp(s2,s1));
}
