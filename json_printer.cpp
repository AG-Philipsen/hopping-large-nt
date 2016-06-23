//Created: 19-08-2014
//Modified: Tue 19 Aug 2014 11:24:48 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"json_printer.hpp"
#include"pm.wilson.h"

#include<sstream>
#include"std_libs/position/position_default_io.hpp"
#include<boost/lexical_cast.hpp>

namespace hop {

const std::string JSONPrinter::prefactor_key = "pref";
const std::string JSONPrinter::denominator_key = "n";
const std::string JSONPrinter::numerator_key = "m";
const std::string JSONPrinter::position_key = "pos";
const std::string JSONPrinter::trace_key = "N_tr";

const std::string JSONPrinter::factor_key = "factors";
const std::string JSONPrinter::terms_key = "terms";


void JSONPrinter::PrintWilsonString(const WilsonString & ws)
{
  term.add(prefactor_key,boost::lexical_cast<std::string>(ws.prefactor));
  term.add(trace_key,ws.number_of_traces);
}

void JSONPrinter::PrintWilsonStringExit(const WilsonString & ws)
{
  term.add_child(factor_key,factor_array);

  wilson_array.push_back( std::make_pair("", term) );

  factor_array.clear();
  term.clear();
}

void JSONPrinter::PrintWilson(const Wilson & w)
{
  boost::property_tree::ptree factor;
  factor.add(denominator_key, boost::lexical_cast<std::string>(w.n));
  factor.add(numerator_key, boost::lexical_cast<std::string>(w.m));
  factor.add(position_key, PositionToString(w.pos));

  factor_array.push_back( std::make_pair("", factor) );
}

void JSONPrinter::WriteToFile(std::string filename)
{
  boost::property_tree::ptree full_tree;
  full_tree.add_child(terms_key,wilson_array);

  boost::property_tree::write_json(filename, full_tree);
};

std::string JSONPrinter::PositionToString(const Position::pos & pos)
{
  std::ostringstream oss;
  Position::DefaultPrinter printer(oss);
  printer.print(pos);

  return oss.str();
}

} //Namespace hop
