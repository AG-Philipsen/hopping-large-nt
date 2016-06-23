//Created: 04-09-2013
//Modified: Tue 19 Aug 2014 11:30:50 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PMCONFIG_H
#define PMCONFIG_H

#include<fstream>
#include<algorithm>

#include<cstdlib>

#include<vector>
#include<list>
#include<map>
#include<array>
#include<initializer_list>

#include<boost/rational.hpp>
#include<boost/multiprecision/cpp_int.hpp>

#include"std_libs/std_funcs.h"
#include"std_libs/error.h"
#include"std_libs/gamma_prod.h"

#include"pm.typedef.h"
#include"pm.paths.h"
#include"pm.utility.h"

#include"collector.hpp"
#include"printers.hpp"

namespace hop{

//Using statements only for debugging, should be removed
using namespace Debug_Space;

class PMN;

/* ====================================================================================================================================
 * A class containing a possible configuration of Ps and Ms, with a vector of all possible space-time paths of the saved configuration.
 * The class also contains an array saving possible gamma-matrix configurations [to be coded].
 * A description of the individual member functions can be found along the functional implementation in the accompanying cpp-file.
 * ====================================================================================================================================*/

class PMConfig
{

friend class PMN;

// ------------------------------------------------------------------------------------------------------------------------------------

private:
	std::vector<Config> cfgArray;
	std::vector<int> trace_points;
	
	std::list<PMPath> paths;

	boost::rational<PM::pref_type> prefactor;	

// ------------------------------------------------------------------------------------------------------------------------------------

public:
	PMConfig() : prefactor(1) {};
	PMConfig(int len) : cfgArray(len,Cfg_M), prefactor(1) {};
	PMConfig(const Config *cfg, int len) : cfgArray(cfg,cfg+len), prefactor(1) {};
	PMConfig(std::initializer_list<Config> l) : cfgArray(l), prefactor(1) {};
	PMConfig(const PMConfig &rhs) : cfgArray(rhs.cfgArray), trace_points(rhs.trace_points), paths(rhs.paths), 
					prefactor(rhs.prefactor) {};
	~PMConfig() {};

	/* ---------------------------------------------------------------------------------------------------
	 *
	 * Operator overloads, most of which modify the prefactor member variable. The strongEqual function 
	 * checks equality but ignores trace cycling invariance.
	 */

	PMConfig &operator=(const PMConfig&);

	Config& operator[](int i) {return cfgArray[i];};
	const Config& operator[](int i) const {return cfgArray[i];};

	Config& at(int i) {return cfgArray.at(i);};
	const Config& at(int i) const {return cfgArray.at(i);};
	
	friend bool operator==(const PMConfig&,const PMConfig&);
	friend bool strongEqual(const PMConfig &lhs, const PMConfig &rhs) {return lhs.cfgArray == rhs.cfgArray;};

	void operator++()    {++prefactor;};
	void operator++(int) {prefactor++;};

	void operator*=(const PM::pref_type &x) {prefactor *= x;};
	void operator*=(const boost::rational<PM::pref_type> &x) {prefactor *= x;};
	void operator/=(const PM::pref_type &x) {prefactor /= x;};
	void operator/=(const boost::rational<PM::pref_type> &x) {prefactor /= x;};

	void operator*=(const PMConfig&);

	/* ---------------------------------------------------------------------------------------------------
	 *
	 * Utility functions used to e.g. read out the state of your configuration, cycle the p's and m's, 
	 * check in which trace region a p or m is and so forth. Also set and get functions.
	 */

	int getLen() const {return cfgArray.size();};
	int numberOfTraces() const {return trace_points.size();};
	bool is_single() const {return (trace_points.size() <= 1);};

	std::list<PMPath>::const_iterator pbegin() const {return paths.begin();};
	std::list<PMPath>::const_iterator pend() const {return paths.end();};

	void set_prefactor(boost::rational<PM::pref_type> &x) {prefactor = x;};
	boost::rational<PM::pref_type> get_prefactor() const {return prefactor;};

	void finalise() {trace_points.push_back(cfgArray.size());};

	int upper_trace_bound(int) const;
	int lower_trace_bound(int) const;
	//int trace_index(int);

	void ordering();
	void rotate();
	void rotate(int);

	void collect(Collector & coll)
	{
		coll.configCollector(this);

		for(PMPath & p : paths)
			p.collect(coll);
	};

  void print(Printer & printer) const
  {
    printer.PrintPMConfig(*this);

    for(const PMPath & path : paths)
      path.print(printer);

    printer.PrintPMConfigExit(*this);
  };

	/* ---------------------------------------------------------------------------------------------------
	 * Functions related to the special properties of the paths such as special time orderings for
	 * restricted spatial paths, and finding duplicated of spatial paths. Implemented in pm.config.path.cpp
	 */

	void populatePaths();

	void complete_single_trace();
	void complete_multi_trace();

	void fill_delta() {if(is_single()){complete_single_trace();}else{complete_multi_trace();}};

	void gaugeIntegrate()
  {
		for(PMPath &p : paths){
			p.gaugeIntegrate(trace_points);
		}
	};

	/* ---------------------------------------------------------------------------------------------------
	 * Functions related to the calculation of the Gamma-matrices. Needs in general not be called
 	 * as we found out they only contribute a constant factor pow(2,n+1) at order k^n
 	 * Implemented in "pm.config.gamma.cpp"
	 */

	void temporalGammaTrace(const PMPath&);
	std::list<Gamma> spatialGammaTrace(const PMPath&, const int*);
	void gammaTrace();

	void cleanup();

/* ---------------------------------------------------------------------------------------------------------
 * Another private block for recursive functions only used internally. They should not be visible to the 
 * external user as they are never called outside of their bounding function.
 * --------------------------------------------------------------------------------------------------------- */
	
private:

	void temporal_connections(int,int,std::vector<char>&,int); 
	void spatial_contractions_full(int from, int to, PMPath &path, std::vector<char> &used);
	void positional_delta(int,int,PMPath&);

	void fill_external_links(std::vector<char>&,std::list<PMPath>::const_iterator) const;
	bool valid_multi_tr_path(const PMPath&);

	void single_path_crossing(int,int,PMPath&);
};

/* ====================================================================================================================================
 * My own standard error class derived from the class defined in "error.h", more or less the same as <error>
 * ==================================================================================================================================== */
 
class PMConfigError : public Error
{
public:
	PMConfigError(const char *inerr) : Error(inerr,"pmconfig") {}
};

};

#endif
