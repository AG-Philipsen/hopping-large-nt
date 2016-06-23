//Created: 19-08-2014
//Modified: Tue 19 Aug 2014 10:59:07 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef JSON_PRINTER_HPP
#define JSON_PRINTER_HPP

#include"printers.hpp"
#include<string>

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>

namespace Position {
class pos;
}

namespace hop {

class JSONPrinter : public Printer
{
private:

  boost::property_tree::ptree wilson_array;
  boost::property_tree::ptree term;
  boost::property_tree::ptree factor_array;



  static const std::string prefactor_key;
  static const std::string denominator_key;
  static const std::string numerator_key;
  static const std::string position_key;
  static const std::string trace_key;

  static const std::string factor_key;
  static const std::string terms_key;

  std::string PositionToString(const Position::pos &);

public:
  virtual void PrintWilsonString(const WilsonString &);
  virtual void PrintWilsonStringExit(const WilsonString &);
  virtual void PrintWilson(const Wilson &);

  void WriteToFile(std::string filename);
};


}; //Namespace hop



#endif /* JSON_PRINTER_HPP */
