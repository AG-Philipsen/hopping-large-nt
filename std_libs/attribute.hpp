/*
 * Created: 20-11-2014
 * Modified: Sat 22 Nov 2014 16:22:03 CET
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef ATTRIBUTE_HPP
#define ATTRIBUTE_HPP

#include<ostream>
#include<utility>

namespace Utility {

template <
  typename Type, 
  typename Attribute
>
class AttributePair
{
private:
	Type object;
	mutable Attribute attrib;

public:
	const Type & obj() const
	{
		return object;
	}

  Type & obj()
  {
    return object;
  }

	Attribute & attribute() const
	{
		return attrib;
	}

	friend void swap(AttributePair & lhs, AttributePair & rhs)
	{
		using std::swap;
		swap(lhs.object, rhs.object);
		swap(lhs.object_count,rhs.object_count);
	}

  AttributePair & operator=(const AttributePair &) = default;
  AttributePair & operator=(AttributePair &&) = default;

	AttributePair() {}

  AttributePair(const AttributePair &) = default;
  AttributePair(AttributePair &&) = default;

	AttributePair(Type object, Attribute attribute = Attribute {})
    : object(object),
      attrib(attribute) {};
};

template <
  typename Type,
  typename Attribute
>
bool operator<(const AttributePair<Type,Attribute> & lhs,
               const AttributePair<Type,Attribute> & rhs)
{
  return lhs.obj() < rhs.obj();
};

template <
  typename Type,
  typename Attribute
>
bool operator==(const AttributePair<Type,Attribute> & lhs,
               const AttributePair<Type,Attribute> & rhs)
{
  return lhs.obj() == rhs.obj();
};

template <
  typename Type,
  typename Attribute
>
bool operator!=(const AttributePair<Type,Attribute> & lhs,
               const AttributePair<Type,Attribute> & rhs)
{
  return lhs.obj() != rhs.obj();
};

template <
  typename Type,
  typename Attribute
>
bool operator<(const AttributePair<Type,Attribute> & lhs,
               const Type & rhs)
{
  return lhs.obj() < rhs;
};

template <
  typename Type,
  typename Attribute
>
bool operator==(const AttributePair<Type,Attribute> & lhs,
                const Type & rhs)
{
  return lhs.obj() == rhs;
};

template <
  typename Type,
  typename Attribute
>
bool operator!=(const AttributePair<Type,Attribute> & lhs,
                const Type & rhs)
{
  return lhs.obj() != rhs;
};

template <
  typename Type,
  typename Attribute
>
std::ostream & operator<<(std::ostream & os,
                          const AttributePair<Type,Attribute> & rhs)
{
  os << rhs.obj() << ": " << rhs.attribute();
  return os;
};

template <
  typename Type,
  typename Attribute
>
struct AttributePairFullCompare
{
  bool operator() (const AttributePair<Type,Attribute> & lhs,
                   const AttributePair<Type,Attribute> & rhs)
  {
    if (lhs != rhs)
      return lhs < rhs;
    else
      return lhs.attribute() < rhs.attribute();
  }
};

} //namespace Utility

#endif /* ATTRIBUTE_HPP */
