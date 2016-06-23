//Created: 26-09-2014
//Modified: Wed 15 Oct 2014 11:36:58 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"../counted_object.hpp"
#include<gtest/gtest.h>
#include<string>

TEST(CountedObjectTest, DefaultConstructor)
{
  Utility::Counted<int> c_obj;
  EXPECT_EQ(0,c_obj.count());
}

TEST(CountedObjectTest, CopyConstructor)
{
  auto test_string = std::string { "Hello world!" };
  auto c_obj = Utility::Counted<std::string> { test_string };

  EXPECT_EQ(test_string, c_obj.obj());
  EXPECT_EQ(1, c_obj.count());
}

TEST(CountedObjectTest, MoveConstructor)
{
  auto test_string = std::string {"Hello world!"};
  auto test_string_copy = test_string;

  auto c_obj = Utility::Counted<std::string> { std::move(test_string) };

  EXPECT_EQ(test_string_copy, c_obj.obj());
  EXPECT_EQ(1, c_obj.count());

  EXPECT_EQ(true, test_string.empty());
}

TEST(CountedObjectTest, InitialCountConstructor)
{
  const int init_count = 5;

  auto c_obj = Utility::Counted<std::string> { "Hello", init_count };

  EXPECT_EQ(init_count, c_obj.count());
}

TEST(CountedObjectTest, ObjectReference)
{
  auto c_obj = Utility::Counted<int> { 1 };

  EXPECT_EQ(1, c_obj.obj());
  
  c_obj.object_ref() *= 5;
  EXPECT_EQ(5, c_obj.obj());

  c_obj.object_ref() = 256;
  EXPECT_EQ(256, c_obj.obj());
}

TEST(CountedObjectTest, CountReference)
{
  auto c_obj = Utility::Counted<int> { 0 };

  EXPECT_EQ(1, c_obj.count());

  c_obj.count_ref() *= 5;
  EXPECT_EQ(5, c_obj.count());

  c_obj.count_ref() = 20;
  EXPECT_EQ(20, c_obj.count());
}

TEST(CountedObjectTest, IncrDecrOperators)
{
  auto c_obj = Utility::Counted<int> { 0 };

  for(int i = 0; i < 5; ++i)
    c_obj.Increment();

  EXPECT_EQ(6, c_obj.count());

  for(int i = 0; i < 3; ++i)
    c_obj.Increment(5);

  EXPECT_EQ(21, c_obj.count());

  for(int i = 0; i < 3; ++i)
    c_obj.Decrement();

  EXPECT_EQ(18, c_obj.count());

  for(int i = 0; i < 6; ++i)
    c_obj.Decrement(3);

  EXPECT_EQ(0, c_obj.count());
}
  
TEST(CountedObjectTest, ComparisonOpterator)
{
  auto a10 = Utility::Counted<int> { 10 };
  auto a20 = Utility::Counted<int> { 20 };

  auto b10 = Utility::Counted<int> { 10 };

  b10.Increment();

  EXPECT_LT(a10, a20);
  EXPECT_FALSE(a10 < b10);
  EXPECT_TRUE(a10 == b10);
  EXPECT_FALSE(a10 != b10);

  EXPECT_FALSE(a10 < 5);
  EXPECT_TRUE(a10 == 10);
  EXPECT_TRUE(a10 != 20);
}

TEST(CountedObjectFullCompareTest, EqualElements)
{
  auto a = Utility::Counted<int> { 10 };
  auto b = Utility::Counted<int> { 10 };

  auto comp = Utility::CountedFullCompare<int> {};

  EXPECT_FALSE(comp(a,b));

  a.Increment();
  b.Increment();

  EXPECT_FALSE(comp(a,b));
}

TEST(CountedObjectFullCompareTest, DifferentObjects)
{
  auto a = Utility::Counted<int> { 10 };
  auto b = Utility::Counted<int> { 20 };

  auto comp = Utility::CountedFullCompare<int> {};

  EXPECT_TRUE(comp(a,b));
  EXPECT_FALSE(comp(b,a));

  a.Increment();

  EXPECT_TRUE(comp(a,b));
  EXPECT_FALSE(comp(b,a));
}

TEST(CountedObjectFullCompareTest, DifferentCount)
{
  auto a = Utility::Counted<int> { 10 };
  auto b = Utility::Counted<int> { 10 };

  a.Increment();

  auto comp = Utility::CountedFullCompare<int> {};

  EXPECT_FALSE(comp(a,b));
  EXPECT_TRUE(comp(b,a));
}
