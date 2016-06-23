//Created: 09-09-2013
//Modified: Tue 19 Aug 2014 11:32:31 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PMPATHS_H
#define PMPATHS_H

#include<fstream>

#include<cstdlib>
#include<cctype>
#include<cinttypes>

#include<vector>
#include<memory>
#include<list>
#include<set>
#include<map>

#include<boost/math/common_factor.hpp>
#include<boost/rational.hpp>
#include<boost/multiprecision/cpp_int.hpp>

#include"std_libs/std_funcs.h"
#include"std_libs/math_structs.h"
#include"std_libs/error.h"
#include"std_libs/position/position_class.hpp"
#include"std_libs/permutations.h"

#include"pm.typedef.h"
#include"pm.utility.h"
#include"pm.wilson.h"

#include"collector.hpp"
#include"printers.hpp"

namespace hop {

//For debugging, remember to remove it
using namespace Debug_Space;

class PMConfig;
class PMN;

//// A class that contains one specific path corresponding to a configuration of Ps and Ms. This class is only used as a member of the class PMConfig, in which we have one 
//// PMPath for every valid space-time path. The class comes with a self-check to whether or not a special spatial configuration must take place for the path not to contain
//// spatial loops. This function is called crossing(), and it also fills up the class's vector of Kronecker deltas.

class PMPath
{

friend class PMConfig;
friend class PMN;

friend class TermCollector;

friend void rectify_spatial_path(Position::pos &,PMPath &p,std::list< std::vector<int> >::iterator);

private:
	std::vector<int> path;
	std::list< std::vector<int> > s_paths;
	std::list< std::vector<Position::pos> > trace_disp;

	KD_Expr deltas;

	std::list<WilsonString> w;

	static int print_delta;
	static int print_wilson;


public:
	PMPath(int len) : path(len,0), s_paths(1,std::vector<int>(len,0)) {};
	PMPath(const int *dat, int len) : path(dat,dat+len), s_paths(1,std::vector<int>(dat,dat+len)) {};
	PMPath(const PMPath &rhs) : path(rhs.path), s_paths(rhs.s_paths), trace_disp(rhs.trace_disp), deltas(rhs.deltas), w(rhs.w) {};
	~PMPath() {};

	int& operator[](int i) {return path[i];}; 
	const int& operator[](int i) const {return path[i];};

	int& at(int i) {return path.at(i);};
	const int& at(int i) const {return path.at(i);};

	//friend bool operator==(const PMPath&,const PMPath&);
	PMPath& operator=(const PMPath&);
	PMPath& operator*=(const KD_Expr &kde) {deltas *= kde; return *this;};

	//Simple path-operators, changing the path slightly
	int size() const {return path.size();};

	int findLink(int) const;
	void remove_special_spatial_paths(const std::vector<int>&);

	//Functions related to the filling of the kronecker deltas
	//bool crossing();
	void appendDeltas(const KD_Term &new_deltas) {deltas *= new_deltas;};

	//Functions related to the gauge-integral, implemented in pm.paths.gauge.cpp
	void gaugeIntegrate();
	void gaugeIntegrate(const std::vector<int>&);
	std::vector< std::vector<int> > set_wilson_positions(const std::vector<int> &trace_points, const std::vector<int> &indecies);
	//void translateCoordinates();
	
	void collect(Collector & coll)
	{
		coll.pathCollector(this);
	};

  void print(Printer & printer) const
  {
    printer.PrintPMPath(*this);

    for(const auto & spatial : s_paths)
      printer.PrintPathList(spatial);

    for(const WilsonString & wil : w)
      wil.print(printer);

    printer.PrintPMPathExit(*this);
  };

	//Trace-related functions, mostly trace-displacements, and the calculation of them
	
	void fill_trace_displacements(const std::vector<int> &trp);
	std::list< std::vector<int> >::iterator fix_spatial_path(Position::pos &,std::list< std::vector<int> >::iterator);
	bool valid_multi_tr_path(const std::vector<Config> &cfgArray, const std::vector<int> &trace_points);

	template <class PosType, class PathType>
	bool calc_trace_displacements(std::vector<PosType>&,const PosType&, PosType&,const std::vector<int>&,const std::vector<PathType>&);
};

class PMPathError : public Error
{
public:
	PMPathError(const char *inerr) : Error(inerr,"pmpath") {}
};


}; //Namespace hop


#endif
