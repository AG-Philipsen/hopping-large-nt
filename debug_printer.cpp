//Created: 18-08-2014
//Modified: Wed 10 Dec 2014 10:37:06 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
#include"debug_printer.hpp"

#include"pm.config.h"
#include"pm.paths.h"

namespace hop {

void DebugPrinter::PrintPMConfig(const PMConfig & conf)
{
  int next_trace_index = conf.upper_trace_bound(0);

  for(int i = 0; i < conf.getLen(); ++i) {

    if(i == next_trace_index) {
      os << '.';
      next_trace_index = conf.upper_trace_bound(i);
    }

    os << ( (conf[i] == Cfg_P) ? 'p' : 'm' );
  }

  os << " (" << conf.get_prefactor() << ")" << std::endl;
};

void DebugPrinter::PrintPMConfigExit(const PMConfig &)
{
  os << std::endl;
};

void DebugPrinter::PrintPMPath(const PMPath & path)
{
  int i_char = (int)'i' - 1;

  os << "{" << (char)(i_char + abs(path[0]));

  for(int i = 1; i < path.size(); ++i)
    os << ',' << (char)(i_char + abs(path[i]));

  os << "}" << std::endl;
};

void DebugPrinter::PrintPMPathExit(const PMPath &)
{
  os << std::endl;
};

void DebugPrinter::PrintPathList(const std::vector<int> & path)
{
  int i_char = (int)'i' - 1;

  os << "    {" << (char)(i_char + abs(path[0]));

  for(int i = 1; i < path.size(); ++i)
    os << ',' << (char)(i_char + abs(path[i]));

  os << "}" << std::endl;
};

void DebugPrinter::PrintWilsonString(const WilsonString &)
{
  os << '\t';
};

void DebugPrinter::PrintWilsonStringExit(const WilsonString & ws)
{
  os << " (" << ws.prefactor << " Nf";
  if (ws.number_of_traces > 1)
    os << "^" << ws.number_of_traces;
  os << ")" << std::endl;
};

void DebugPrinter::PrintWilson(const Wilson & w)
{
  os << "W(" << w.n << "," << w.m << ",";
  position_printer->print(w.pos);
  os << ")";
};

} //Namespace hop
