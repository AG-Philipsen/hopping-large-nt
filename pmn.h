//Created: 18-09-2013
//Modified: Tue 19 Aug 2014 11:29:51 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PMN_H
#define PMN_H

#include<fstream>

#include<list>
#include<vector>
#include<set>
#include<memory>

#include<cinttypes>

#include<boost/math/common_factor.hpp>
#include<boost/rational.hpp>

#include"pm.typedef.h"
#include"pm.config.h"

#include"std_libs/std_funcs.h"
#include"std_libs/error.h"

#include"printers.hpp"
#include"collector.hpp"

namespace hop{

using boost::math::gcd;

struct WilsonStringPtr{
	const WilsonString *ptr;
	boost::rational<PM::pref_type> prefactor;

	~WilsonStringPtr();
};

//// Class which saves all unique pmConfigurations to all orders up to "order", or only the highest order if 
//// allOrders = false. It also has a list of PMComb's, which is all multi-trace configurations at the highest
//// order. This list can only be filled if allOrders = true, as it needs the results from the lower order
//// configurations

class PMN{
private:
	
	std::vector< std::vector<PMConfig> > configurations;

	std::vector<PMConfig>::iterator multi_trace_begin;
	std::vector<PMConfig>::const_iterator multi_trace_begin_const;

	int order;

public:
	PMN(int);

	//Functions related to the filling of the single-trace configurations
	void fillConfigs();
	void fillPaths();

	//Functions related to the alternative printing
	void fillTermsList();
	void contractWilsons();
	void printWilsonTerms(FILE *out=stdout, bool printCheck = true) const;

	void collect(Collector & coll)
	{
		for(PMConfig & conf : configurations.back())
			conf.collect(coll);
	};

  void print(Printer & printer) const
  {
    printer.PrintPMN(*this);

    for(const PMConfig & conf : configurations.back())
      conf.print(printer);

    printer.PrintPMNExit(*this);
  };

	//Contains an integer i and how many times it has been counted
	struct intCount{
		int i;
		int c;
	};

private:

	//Function which fills up configurations and multiTrConf
	int fillAllConfigs(int,int,int,std::vector<PMConfig>&,int);
	void fill_multi_config(const std::vector<int>&,int,int,std::list< std::vector<int> >);
};

class PMNError : public Error {
public:
	PMNError(const char *inerr) : Error(inerr,"pmn") {};
};

}; //Namespace hop

#endif
