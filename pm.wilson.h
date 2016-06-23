//Created: 12-03-2014
//Modified: Tue 19 Aug 2014 11:33:21 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PM_WILSON_H
#define PM_WILSON_H

#include<utility>
#include<fstream>

#include<vector>
#include<list>

#include<boost/rational.hpp>

#include"std_libs/error.h"
#include"std_libs/position/position_class.hpp"
#include"pm.typedef.h"

#include"printers.hpp"

namespace hop{

//// Simple struct which contains a single Wilson line, with winding number. Wilson::pos, num, powerof is equal to W(num,pos)
struct Wilson
{
	unsigned n, m;
  Position::pos pos;

	friend void swap(Wilson & lhs, Wilson & rhs)
	{
		using std::swap;

		swap(lhs.n, rhs.n);
		swap(lhs.m, rhs.m);
		swap(lhs.pos,rhs.pos);
	};

	Wilson() : n(0), m(0) {};
	Wilson(const Wilson &rhs) : n(rhs.n), m(rhs.m), pos(rhs.pos) {};
	Wilson(Wilson &&rhs) : Wilson()
	{
		swap(*this,rhs);
	};

	Wilson& operator=(Wilson rhs)
	{
		swap(*this,rhs);
		return *this;
	};

  void print(Printer & printer) const
  {
    printer.PrintWilson(*this);
  };

	bool operator< (const Wilson&) const;
};

struct WilsonString
{
	std::vector<Wilson> wilsons;
	boost::rational<PM::pref_type> prefactor;
	int number_of_traces;

	friend void swap(WilsonString & lhs, WilsonString & rhs)
	{
		using std::swap;

		swap(lhs.wilsons, rhs.wilsons);
		swap(lhs.prefactor, rhs.prefactor);
		swap(lhs.number_of_traces, rhs.number_of_traces);
	};

	WilsonString() : prefactor(1), number_of_traces(0) {};
	WilsonString(const WilsonString &rhs) : wilsons(rhs.wilsons), prefactor(rhs.prefactor), 
		number_of_traces(rhs.number_of_traces) {};
	WilsonString(WilsonString &&rhs) : WilsonString()
	{
		swap(*this,rhs);
	};

	WilsonString& operator=(WilsonString rhs)
	{
		swap(*this,rhs);
		return *this;
	};

	void coordinateCleanup();

  void print(Printer & printer) const
  {
    printer.PrintWilsonString(*this);

    for(const Wilson & w : wilsons)
      w.print(printer);

    printer.PrintWilsonStringExit(*this);
  };

	bool operator<(const WilsonString & rhs) const
	{
		return wilsons < rhs.wilsons;
	};
};

}; //Namespace hop


#endif /* PM_WILSON_H */
