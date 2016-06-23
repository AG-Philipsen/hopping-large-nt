//Created: 13-05-2014
//Modified: Tue 13 May 2014 17:03:30 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef STD_LIBS_TYPEDEFS_HPP
#define STD_LIBS_TYPEDEFS_HPP

#include<type_traits>

template <typename Condition, typename T = void>
using EnableIf = typename std::enable_if<Condition::value, T>::type;

template <typename Condition, typename T = void>
using DisableIf = typename std::enable_if<!Condition::value, T>::type;

namespace std_types {

template <class Type>
struct LessThanOrEqualToZero
{
	bool operator() (const Type & val)
	{
		return val <= (Type)0;
	};
};

} //Namespace std_types

#endif /* STD_LIBS_TYPEDEFS_HPP */
