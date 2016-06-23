/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 12:31:18 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef LEXICOGRAPHIC_HPP
#define LEXICOGRAPHIC_HPP

#include<functional>
#include<ostream>
#include"template_structs.hpp"

namespace Utility {
namespace Permutation{

/// Permutations using lexicgraphic ordering
template <
  typename Container,
  typename Compare = std::less<typename Container::iterator::value_type>
>
class Lexicographic
{
private:
  typedef typename Container::iterator iterator;

  iterator first, last;
	Compare value_compare;
	bool last_element_reached;

	iterator swap_low, swap_high;

	void FindLargestSortedIndex()
  {
    iterator it1 = last; --it1;
    iterator it2 = it1;

    while(it1 != first) {
      --it1;

      if(value_compare(*it1,*it2)) {
        swap_low = it1;
        return;
      }

      --it2;
    }

		last_element_reached = true;
	}

	void FindLargestGreaterIndex()
  {
    iterator it = last; --it;
    for(; it != swap_low; --it) {
      if(value_compare(*swap_low,*it)) {
       swap_high = it;
       return;
      }
    }

  };

public:
	void operator++()
  {

    if(last_element_reached)
      return;

		FindLargestSortedIndex();

		if(last_element_reached)
			return;

		FindLargestGreaterIndex();

    IteratorSwapper<Container>::swap(swap_low,swap_high);
    IteratorReverser<Container>::reverse(++swap_low,last);
	}

	bool operator!() const
  {
    return !last_element_reached;
  };

  operator bool() const 
  {
    return last_element_reached;
  };

  friend std::ostream & operator<<(
      std::ostream & os, 
      const Lexicographic<Container,Compare> & l)
  {
    os << "[";

    for(auto it = l.first; it != l.last; ++it)
      os << *it << ",";

    os << "]";

    return os;
  };


	Lexicographic(iterator begin, iterator end, const Compare &comp = Compare()) 
    : first(begin),
      last(end),
      value_compare(comp),
      last_element_reached(begin == end) {};
};


}}

#endif /* LEXICOGRAPHIC_HPP */
