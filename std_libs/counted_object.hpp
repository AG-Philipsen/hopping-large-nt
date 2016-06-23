//Created: 08-07-2014
//Modified: Wed 25 Feb 2015 13:08:42 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef COUNTED_OBJECT_HPP
#define COUNTED_OBJECT_HPP

#include"attribute.hpp"

namespace Utility {

/*! \brief Combines any object with an object count.
 *
 * Effectively a std::pair<Type,int> where the integer is meant to represent
 * the count of the object being counted. The count is represented by a
 * mutable integer so that the count doesn't affect the class's const-ness, and
 * so that counts can technically be negative if that is required.
 *
 * The class can be used anywhere one needs to keep track of how many e.g.
 * copies there are of an object. It is also the base of the CountedSet class.
 *
 * The class is implemented in a header-only library, as should all utility
 * template classes.
 */
template <typename Type>
class Counted : public AttributePair<Type,int>
{
public:
  /*! \brief Returns a pure reference to the counted object */
  Type & object_ref()
  {
    return this->obj();
  };

  /*! \brief Returns the current count */
	const int count() const
	{
		return this->attribute();
	};

  /*! \brief Returns a reference to the current count */
	int & count_ref() const
	{
		return this->attribute();
	};

  /*! \brief Increments the current count by an arbitrary amount, default = 1 */
	void Increment(int incrementation = 1) const
	{
    this->attribute() += incrementation;
	};

  /*! \brief Decrements the current count by an arbitrary amount, default = 1 */
	void Decrement(int decrementation = 1) const
	{
    this->attribute() -= decrementation;
	};

  Counted & operator=(const Counted &) = default;
  Counted & operator=(Counted &&) = default;

  /*! \breif Empty object has a default count of zero. */
	Counted()
    : AttributePair<Type,int>::AttributePair(Type(), 0) {};

  Counted(const Counted &) = default;
  Counted(Counted &&) = default;

  /*! \brief Constructs the counted object with an object and a count.
   *
   * This constructor can be used for implicit converstions between any
   * object and its counted variant, in which case the count will be one.
   */
	Counted(Type object, int init_count = 1)
    : AttributePair<Type,int>::AttributePair(object, init_count) {};
};

/*! \brief Strict comparison operator for Counted objects.
 *
 * Does not ignore count when comparing two Counted objects, however
 * the count is taken into account only if the objects are equal.
 *
 * Requires that the counted object has an overloaded != operator.
 */
template <typename Type>
using CountedFullCompare = AttributePairFullCompare<Type,int>;

} //Namespace Utility

#endif /* COUNTED_OBJECT_HPP */
