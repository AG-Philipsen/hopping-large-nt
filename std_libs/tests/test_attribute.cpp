/*
 * Created: 22-11-2014
 * Modified: Sat 22 Nov 2014 16:23:41 CET
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<gtest/gtest.h>
#include<attribute.hpp>
#include<set>

using namespace Utility;

typedef AttributePair<std::string, std::string> StringPair;
typedef AttributePair<double, std::string> NumericPair;

TEST(AttributePairTest, DefaultConstructor)
{
  auto attribute_pair = StringPair {};

  EXPECT_EQ(std::string {}, attribute_pair.obj());
  EXPECT_EQ(std::string {}, attribute_pair.attribute());
}

TEST(AttributePairTest, ObjectConstructor)
{
  auto test_string = std::string {"hello"};
  auto attribute_pair = StringPair {test_string};

  EXPECT_EQ(test_string, attribute_pair.obj());
  EXPECT_EQ(std::string {}, attribute_pair.attribute());
}

TEST(AttributePairTest, ObjAttributeConstructor)
{
  auto test_string_1 = std::string {"hello"};
  auto test_string_2 = std::string {"world"};

  auto attribute_pair = StringPair {test_string_1, test_string_2};

  EXPECT_EQ(test_string_1, attribute_pair.obj());
  EXPECT_EQ(test_string_2, attribute_pair.attribute());
}

TEST(AttributePairTest, CopyConstructor)
{
  auto test_string_1 = std::string {"hello"};
  auto test_string_2 = std::string {"world"};

  auto attribute_pair = StringPair {test_string_1, test_string_2};
  auto attribute_pair_cpy = StringPair {attribute_pair};

  EXPECT_EQ(test_string_1, attribute_pair.obj());
  EXPECT_EQ(test_string_2, attribute_pair.attribute());

  EXPECT_EQ(test_string_1, attribute_pair_cpy.obj());
  EXPECT_EQ(test_string_2, attribute_pair_cpy.attribute());
}

TEST(AttributePairTest, MoveConstructor)
{
  auto test_string_1 = std::string {"hello"};
  auto test_string_2 = std::string {"world"};

  auto attribute_pair = StringPair {test_string_1, test_string_2};
  auto attribute_pair_cpy = StringPair {std::move(attribute_pair)};

  EXPECT_EQ(std::string {}, attribute_pair.obj());
  EXPECT_EQ(std::string {}, attribute_pair.attribute());

  EXPECT_EQ(test_string_1, attribute_pair_cpy.obj());
  EXPECT_EQ(test_string_2, attribute_pair_cpy.attribute());
}

TEST(AttributePairTest, ObjectReference)
{
  auto test_string_1 = std::string {"hello"};
  auto test_string_2 = std::string {"world"};
  auto test_string_3 = std::string {"foobar"};

  auto attribute_pair = StringPair {test_string_1, test_string_2};
  attribute_pair.obj() = test_string_3;

  EXPECT_EQ(test_string_3, attribute_pair.obj());
  EXPECT_EQ(test_string_2, attribute_pair.attribute());
}

TEST(AttributePairTest, AttributeReference)
{
  auto test_string_1 = std::string {"hello"};
  auto test_string_2 = std::string {"world"};
  auto test_string_3 = std::string {"foobar"};

  auto attribute_pair = StringPair {test_string_1, test_string_2};

  auto pair_set = std::set<StringPair> {attribute_pair};

  auto it = pair_set.begin();
  ASSERT_NE(it, pair_set.end());

  it->attribute() = test_string_3;

  EXPECT_EQ(test_string_1, it->obj());
  EXPECT_EQ(test_string_3, it->attribute());
}

TEST(AttributePairTest, LessThan)
{
  auto test_string_1 = std::string {"aaaaa"};
  auto test_string_2 = std::string {"bbbbb"};
  auto test_string_3 = std::string {"world"};
  auto test_string_4 = std::string {"foobar"};

  auto attribute_pair_1 = StringPair {test_string_1, test_string_3};
  auto attribute_pair_2 = StringPair {test_string_1, test_string_4};
  auto attribute_pair_3 = StringPair {test_string_2, test_string_4};

  EXPECT_FALSE(attribute_pair_1 < attribute_pair_2);
  EXPECT_FALSE(attribute_pair_2 < attribute_pair_1);

  EXPECT_TRUE(attribute_pair_1 < attribute_pair_3);
  EXPECT_FALSE(attribute_pair_3 < attribute_pair_1);
}

TEST(AttributePairTest, EqualTo)
{
  auto test_string_1 = std::string {"aaaaa"};
  auto test_string_2 = std::string {"bbbbb"};
  auto test_string_3 = std::string {"world"};
  auto test_string_4 = std::string {"foobar"};

  auto attribute_pair_1 = StringPair {test_string_1, test_string_3};
  auto attribute_pair_2 = StringPair {test_string_1, test_string_4};
  auto attribute_pair_3 = StringPair {test_string_2, test_string_4};

  EXPECT_TRUE(attribute_pair_1 == attribute_pair_2);
  EXPECT_FALSE(attribute_pair_1 == attribute_pair_3);
}

TEST(AttributePairTest, NotEqualTo)
{
  auto test_string_1 = std::string {"aaaaa"};
  auto test_string_2 = std::string {"bbbbb"};
  auto test_string_3 = std::string {"world"};
  auto test_string_4 = std::string {"foobar"};

  auto attribute_pair_1 = StringPair {test_string_1, test_string_3};
  auto attribute_pair_2 = StringPair {test_string_1, test_string_4};
  auto attribute_pair_3 = StringPair {test_string_2, test_string_4};

  EXPECT_FALSE(attribute_pair_1 != attribute_pair_2);
  EXPECT_TRUE(attribute_pair_1 != attribute_pair_3);
}
