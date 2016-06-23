//Created: 22-01-2014
//Modified: Fri 23 May 2014 10:23:50 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PM_TYPEDEF_H
#define PM_TYPEDEF_H

#include<boost/multiprecision/cpp_int.hpp>

class PMPath;

namespace PM{
	typedef boost::multiprecision::cpp_int pref_type;
	typedef const PMPath* PMPathPtr;
};

namespace hop{
	enum Config{
		Cfg_M = -1,
		Cfg_P =  1
	};
};

#endif /* PM_TYPEDEF_H */
