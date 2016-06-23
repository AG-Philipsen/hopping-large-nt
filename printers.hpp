//Created: 18-08-2014
//Modified: Mon 18 Aug 2014 15:16:22 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PRINTERS_HPP
#define PRINTERS_HPP

#include<vector>

namespace hop {

class PMN;
class PMConfig;
class PMPath;
struct WilsonString;
struct Wilson;

class Printer
{
public:
  virtual void PrintPMN(const PMN &) {};
  virtual void PrintPMNExit(const PMN &) {};
  virtual void PrintPMConfig(const PMConfig &) {};
  virtual void PrintPMConfigExit(const PMConfig &) {};
  virtual void PrintPMPath(const PMPath &) {};
  virtual void PrintPMPathExit(const PMPath &) {};
  virtual void PrintPathList(const std::vector<int> &) {};
  virtual void PrintWilsonString(const WilsonString &) {};
  virtual void PrintWilsonStringExit(const WilsonString &) {};
  virtual void PrintWilson(const Wilson &) {};
};

}

#endif /* PRINTERS_HPP */
