//Created: 12-03-2014
//Modified: Tue 19 Aug 2014 11:33:29 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"pm.wilson.h"
#include"std_libs/position/position_manip.hpp"
#include"position_wilson_vector.hpp"
#include<utility>

#include<iostream>

namespace hop{

/* ---------- Wilson Struct ---------- */

/// The equality operator checks both the position and the looping coefficient, but not the power

bool Wilson::operator<(const Wilson & rhs) const
{
	if(n != rhs.n)
		return n < rhs.n;

	if(m != rhs.m)
		return m < rhs.m;

	return pos < rhs.pos;
};

/* ---------- WilsonString Struct ---------- */

void WilsonString::coordinateCleanup()
{
	std::sort(wilsons.begin(), wilsons.end());

  Position::pos zero_pos = wilsons.front().pos;

	for(Wilson & w : wilsons)
		w.pos -= zero_pos;

  PositionWilsonVector wilson_to_pos_converter(wilsons);

  Position::Manipulator::CoordinateCleaner cleaner;
  cleaner.execute(
      wilson_to_pos_converter.begin(),
      wilson_to_pos_converter.end());
};

}; //Namespace hop
