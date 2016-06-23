/*
 * Created: 14-10-2014
 * Modified: Tue 14 Oct 2014 13:49:53 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include<position/position_default_io.hpp>
#include<gtest/gtest.h>

namespace Position {

TEST(PositionDefaultPrinterTest, Printing)
{
  auto p1 = pos{1,-2,3};
  auto p2 = pos{0};
  auto p3 = pos{};

  auto s1 = std::string("{1,-2,3}");
  auto s2 = std::string("{0}");
  auto s3 = std::string("{}");

  std::ostringstream oss;
  auto printer = DefaultPrinter{oss};

  printer.print(p1);
  EXPECT_EQ(s1, oss.str());
  oss.str("");

  printer.print(p2);
  EXPECT_EQ(s2, oss.str());
  oss.str("");

  printer.print(p3);
  EXPECT_EQ(s3, oss.str());
}

TEST(PositionSymbolPrinterTest, Printing)
{
  auto p1 = pos{1,-2,3};
  auto p2 = pos{0};
  auto p3 = pos{};

  auto s1 = std::string("x + i - 2*j + 3*k");
  auto s2 = std::string("y");
  auto s3 = std::string("z");

  std::ostringstream oss;
  auto printerX = SymbolPrinter{oss};

  printerX.print(p1);
  EXPECT_EQ(s1, oss.str());
  oss.str("");

  auto printerY = SymbolPrinter{oss,'y'};

  printerY.print(p2);
  EXPECT_EQ(s2, oss.str());
  oss.str("");

  auto printerZ = SymbolPrinter{oss,'z'};

  printerZ.print(p3);
  EXPECT_EQ(s3, oss.str());
}

TEST(PositionPrintParserTest, ParseString)
{
  auto p_print = pos{1,2,-3,8,0,0,142};

  std::stringstream ss;
  auto printer = DefaultPrinter{ss};

  printer.print(p_print);

  auto parser = DefaultParser{};
  auto p_parsed = parser.parseString(ss.str());

  EXPECT_EQ(p_print, p_parsed);
}

TEST(PositionPrintParserTest, ParseStream)
{
  auto p_print = pos{1,2,-3,8,0,0,142};

  std::stringstream ss;
  auto printer = DefaultPrinter{ss};

  printer.print(p_print);

  auto parser = DefaultParser{};
  auto p_parsed = parser.parseStream(ss);

  EXPECT_EQ(p_print, p_parsed);
}

TEST(PositionPrintParserTest, ParseFile)
{
  auto p_print = pos{1,2,-3,8,0,0,142};

  auto filename = std::string("obj/Data/tmp.dat");

  std::ofstream os{filename};

  if(!os)
    throw std::runtime_error("Cannot open file " + filename);

  auto printer = DefaultPrinter{os};

  printer.print(p_print);
  os.close();

  auto parser = DefaultParser{};
  auto p_parsed = parser.parseFile(filename);

  EXPECT_EQ(p_print, p_parsed);
}


}; //Namespace Position
