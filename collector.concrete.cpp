//Modified: Fri 06 Mar 2015 13:53:07 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"collector.concrete.hpp"

#include<utility>

#include"pm.config.h"
#include"pm.paths.h"

#include"std_libs/position/position_compare.hpp"

namespace hop {

void TermCollector::configCollector(PMConfig * object)
{
  current_config_prefactor = object->get_prefactor();
  current_number_of_traces = object->numberOfTraces();
}

void TermCollector::pathCollector(PMPath * object)
{
  for(WilsonString & w_term : object->w) {

    //w_term.coordinateCleanup();
    w_term.prefactor *= current_config_prefactor;
    w_term.number_of_traces = current_number_of_traces;

    auto lower = std::lower_bound(
        terms.begin(), terms.end(), 
        w_term, StrictWilsonStringComparator() );

    if( (lower == terms.end()) or StrictWilsonStringComparator::Compare(w_term,*lower) ) {
      terms.insert(lower, std::move(w_term) );
    } else {
      lower->prefactor += w_term.prefactor;

      if(lower->prefactor == 0)
        terms.erase(lower);
    }
  }
}

void TermCollector::fetchResults(std::list<WilsonString> & res)
{
  res.splice(res.end(), std::move(terms));
}

bool StrictWilsonStringComparator::Compare(
    const WilsonString & lhs, 
    const WilsonString & rhs)
{
  if (lhs.number_of_traces != rhs.number_of_traces)
    return lhs.number_of_traces < rhs.number_of_traces;

  return std::lexicographical_compare(
      lhs.wilsons.begin(), lhs.wilsons.end(),
      rhs.wilsons.begin(), rhs.wilsons.end(),
      StrictWilsonComparator());
};

bool StrictWilsonComparator::Compare(
    const Wilson & lhs,
    const Wilson & rhs)
{
  if(lhs.n != rhs.n)
    return lhs.n < rhs.n;
  else if(lhs.m != rhs.m)
    return lhs.m < rhs.m;

  return lhs.pos < rhs.pos;
  //return Position::StrictCompare::Compare(lhs.pos,rhs.pos);
};

} //Namespace hop
