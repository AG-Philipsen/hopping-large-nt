//Created: 19-08-2014
//Modified: Fri 10 Oct 2014 15:07:21 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_COMPARE_HPP
#define POSITION_COMPARE_HPP

#include"position_class.hpp"
#include<algorithm>

namespace Position {

struct StrictCompare
{
  static bool Compare(const pos & lhs, const pos & rhs)
  {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs),
                                        std::begin(rhs), std::end(rhs));
  };
  
  bool operator() (const pos & lhs, const pos & rhs)
  {
    return Compare(lhs,rhs);
  };
};

struct AbsoluteValueCompare
{
public:
  static bool Compare(const pos & lhs, const pos & rhs)
  {
    size_t lhs_size = lhs.size();
    size_t rhs_size = rhs.size();

    size_t smallest_size = ( lhs_size < rhs_size ) ? lhs_size : rhs_size;

    for(int i = 0; i < smallest_size; ++i) {
      if(std::abs(lhs.at(i)) != std::abs(rhs.at(i)))
        return std::abs(lhs.at(i)) < std::abs(rhs.at(i));
    }

    if(lhs_size < rhs_size) {
      for(int i = lhs_size; i < rhs_size; ++i)
        if(rhs.at(i) != 0)
          return true;
    } else if (rhs_size < lhs_size) {
      for(int i = rhs_size; i < lhs_size; ++i)
        if(lhs.at(i) != 0)
          return false;
    }

    return false;
  };

  bool operator() (const pos & lhs, const pos & rhs)
  {
    return Compare(lhs,rhs);
  };
};

} //Namespace Position

#endif /* POSITION_COMPARE_HPP */
