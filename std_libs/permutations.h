//Created: 12-03-2014
//Modified: Wed 15 Oct 2014 14:40:34 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include<fstream>
#include"lexicographic.hpp"
#include"sjt_permutations.hpp"

namespace Utility {
namespace Permutation{

/*! \brief For-loop construct which picks n elements from N
 *
 * The class is depricated and should be replaced by SubsetPick.
 *
 * On construction it takes a dynamic array, the length and
 * the number of elements to pick. It then populated a
 * one array with the picked elements and one with the
 * remainder.
 *
 * The clas has a flag, last, which is set when the last configuration
 * is met.
 */
struct PickFrom{

	int *all_elements;  //!< Copy of all elements of input-list
	int *pick;          //!< Copy of the elements of the current pick
	int *rest;          //!< Copy of the elements not picked
	int *indecies;

  std::size_t len;            //!< Number of elements to pick from, N
	std::size_t n;              //!< Number of elements in pick, n

	std::size_t current;

	bool last;          //!< Flag, whether or not last pick permutation has been reached

  /*! \brief Copies all input elements and chooses the first pick. */
	PickFrom(const int * elements, std::size_t length, std::size_t n);

  /*! \brief Deletes the four dynamically allocated arrays */
	~PickFrom();

  /*! \brief Increments the current pick to the next permutation */
	void operator++();

  /*! \brief Does exactly the same as the pre-increment operator */
	void operator++(int)
  {
    operator++();
  };
};

/* ---------- PickFrom member functions ---------- */


/*!
 * The first pick consists of the first n elements. The function
 * doesn't do any form of exception handling, even if N and n
 * are invalid with respect to the provided data.
 *
 * @param[in] els The elements to copy from
 * @param[in] length The number of elements to pick from
 * @param[in] number_of_picks Number of elements to pick
 */
inline PickFrom::PickFrom(const int *els, std::size_t length, std::size_t number_of_picks)
  : len(length),
    n(number_of_picks),
    last(false)
{
	all_elements = new int[len];
	for(int i=0; i<len; i++){
		all_elements[i] = els[i];
	}

	pick = new int[n];
	indecies = new int[n];

	for(int i=0; i<n; i++){
		pick[i] = all_elements[i];
		indecies[i] = i;
	}

	rest = new int[len-n];

	for(int i=0; i<(len-n); i++){
		rest[i] = all_elements[i+n];
	}

	current = n-1;
}

inline PickFrom::~PickFrom()
{
	delete [] all_elements;
	delete [] pick;
	delete [] indecies;
	delete [] rest;

}

/*!
 * Looks for the next possible pick of n elements out of all. If a
 * new pick is possible, the member variables pick and rest are changed
 * to their new values, if not the last flag will be set to true.
 */
inline void PickFrom::operator++()
{
	if(current == n-1)
  {
		if(indecies[current] < len-1)
    {
			rest[indecies[current]-n+1] = all_elements[indecies[current]];
			indecies[current]++;
			pick[current] = all_elements[indecies[current]];
			return;
		}
	}
  else if(indecies[current] < indecies[current+1]-1)
  {
		indecies[current]++;
		pick[current] = all_elements[indecies[current]];

		for(int i=(current+1); i<n; i++)
    {
			indecies[i] = indecies[current] + (i-current);
			pick[i] = all_elements[indecies[i]];
		}

		int restCount = 0;
		int ncount = 0;
		for(int i=0; i<len; i++)
    {
			if(i != indecies[ncount])
      {
				rest[restCount] = all_elements[i];
				restCount++;
				continue;
			}

			ncount++;
		}

		current  = n-1;
		return;
	}

	int nextCurrent = current-1;

	bool foundOne = false;

	while(nextCurrent >= 0)
  {
		if( indecies[nextCurrent] < (indecies[nextCurrent+1] - 1) )
    {
			foundOne = true;
			current = nextCurrent;
			break;
		}
		nextCurrent--;
	}

	if(!foundOne){
		last = true;
		return;
	}

	operator++();

}

/*! \brief Prints the PickFrom for-loop construct in a debug-friendly way. */
inline std::ostream &
operator<<(std::ostream &os, const PickFrom &p)
{
	os << "[";

	for(int i=0; i<p.n; i++){
		os << p.pick[i] << ",";
	}

	os << "], [";
	for(int i=0; i<(p.len - p.n); i++){
		os << p.rest[i] << ",";
	}

	os << "]";

	return os;
}

} //Namespace Permutation
} //Namespace Utility

#endif /* PERMUTATIONS_H */
