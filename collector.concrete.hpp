//Created: 23-05-2014
//Modified: Tue 19 Aug 2014 18:13:11 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef COLLECTOR_CONCRETE_HPP
#define COLLECTOR_CONCRETE_HPP

#include"collector.hpp"

#include<list>
#include<algorithm>

#include"pm.wilson.h"

namespace hop {

class TermCollector : public Collector
{
private:
  std::list<WilsonString> terms;
  boost::rational<PM::pref_type> current_config_prefactor;
  int current_number_of_traces;

public:
  virtual void pathCollector(PMPath * path);
  virtual void configCollector(PMConfig * object);

  virtual void fetchResults(std::list<WilsonString> & res);

  virtual ~TermCollector() {};
};

struct StrictWilsonStringComparator
{
  static bool Compare(const WilsonString & lhs, const WilsonString & rhs);

  bool operator() (const WilsonString & lhs, const WilsonString & rhs)
  {
    return Compare(lhs,rhs);
  };
};

struct StrictWilsonComparator
{
  static bool Compare(const Wilson & lhs, const Wilson & rhs);

  bool operator() (const Wilson & lhs, const Wilson & rhs)
  {
    return Compare(lhs,rhs);
  };
};

}; //Namespace hop

#endif /* COLLECTOR_CONCRETE_HPP */
