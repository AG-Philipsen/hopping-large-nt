//Created: 04-09-2013
//Modified: Tue 19 Aug 2014 11:29:20 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
#include<cstdio>
#include<fstream>
#include<cstdlib>
#include<ctime>

#include<vector>
#include<string>

#include<sys/types.h>
#include<sys/stat.h>
using namespace std;

#include<boost/lexical_cast.hpp>

#define BOOST_ALL_NO_LIB 1

#include"std_libs/std_funcs.h"
#include"pmn.h"

#include"debug_printer.hpp"
#include"json_printer.hpp"
#include"collector.concrete.hpp"

int main(int argc, char** argv)
{

  if(argc < 2) {
    cerr << "No order given" << endl;
    return 1;
  }

  int order;
  try{
    order = boost::lexical_cast<int>(argv[1]);
  }catch (boost::bad_lexical_cast &) {
    cout << "The given argument, \"" << argv[1] << "\", is not a number" << endl;
    return 1;
  }

  hop::PMN pmn(order);
  pmn.fillConfigs();
  pmn.fillPaths();

  string foldername = "Configurations";
  { //checking if the folder exists, and create it if it doesn't
    struct stat st {0};
    if(stat(foldername.c_str(), &st) == -1)
      mkdir(foldername.c_str(), 0755);
  }

  string filename = foldername + "/kappa" + boost::lexical_cast<string>(order) + ".debug";

  ofstream out;
  out.open(filename);

  hop::DebugPrinter debug_printer(out, new Position::SymbolPrinter(out));
  pmn.print(debug_printer);

  out.close();

  hop::TermCollector collector;
  pmn.collect(collector);

  std::list<hop::WilsonString> terms;
  collector.fetchResults(terms);

  filename = foldername + "/kappa" + boost::lexical_cast<string>(order) + ".terms";

  out.open(filename);
  hop::DebugPrinter term_printer(out, new Position::SymbolPrinter(out));

  for(const hop::WilsonString & w : terms)
    w.print(term_printer);

  out.close();

  filename = foldername + "/kappa" + boost::lexical_cast<string>(order) + ".json";

  hop::JSONPrinter json_printer;

  for(const hop::WilsonString & ws : terms)
    ws.print(json_printer);

  json_printer.WriteToFile(filename);

}
