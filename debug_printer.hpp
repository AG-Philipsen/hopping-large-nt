//Created: 18-08-2014
//Modified: Tue 19 Aug 2014 11:19:48 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef DEBUG_PRINTER_HPP
#define DEBUG_PRINTER_HPP

#include"printers.hpp"
#include<iostream>
#include<memory>
#include"std_libs/position/position_io.hpp"
#include"std_libs/position/position_default_io.hpp"

namespace hop {

class DebugPrinter : public Printer
{
private:
  std::ostream & os;
  std::unique_ptr<Position::Printer> position_printer;

public:
  virtual void PrintPMConfig(const PMConfig &);
  virtual void PrintPMConfigExit(const PMConfig &);
  virtual void PrintPMPath(const PMPath &);
  virtual void PrintPMPathExit(const PMPath &);
  virtual void PrintPathList(const std::vector<int> &);
  virtual void PrintWilsonString(const WilsonString &);
  virtual void PrintWilsonStringExit(const WilsonString &);
  virtual void PrintWilson(const Wilson &);

  DebugPrinter(std::ostream & os = std::cout)
    : os(os),
      position_printer(new Position::DefaultPrinter(os)) {};

  DebugPrinter(std::ostream & os, Position::Printer * printer)
    : os(os),
      position_printer(printer) {};

  virtual ~DebugPrinter() {};
};



};

#endif /* DEBUG_PRINTER_HPP */
