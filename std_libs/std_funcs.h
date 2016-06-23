//Created: 27-09-2013
//Modified: Wed 15 Oct 2014 15:09:55 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef STDFUNCS_H
#define STDFUNCS_H

#include<cstdio>
#include<list>
#include<vector>
#include<string>
#include<array>
#include<algorithm>

#include"error.h"
#include"typedefs.hpp"


/*!
 * Useful standard functions and complicated classes which does standard
 * operations I haven't found an external library to do.
 */
namespace Utility {

/*! \brief Calculates the sign of any type. Returns {-1,0,1} */
template <class Type>
inline int sign(const Type & x)
{
	return (x > (Type)0) - (x < (Type)0);
};

/*! \brief Calculates the sign og any type. sign_w_zero(0) = 1. */
template <class Type>
inline int sign_w_zero(const Type & x)
{
	return (x >= (Type)0) - (x < (Type)0);
};

/*!
 * Calculates the integer power of any type using recursion.
 * @param[in] x Value to take the power of.
 * @param[in] n The integer power
 */
template <class Type>
inline Type intPow(const Type & x, int n)
{
	if(n == 0){
		return 1;
	}else if(n>0){
		return x*intPow(x, n-1);
	}else{
		return intPow(x, n+1)/x;
	}
};

/*!
 * Take the integer factorial of any type, where the return type must
 * be specified as a function template when it is called.
 */
template <class Type>
inline Type Factorial(unsigned int n)
{
	Type result(1);

	for(unsigned int i = 1; i <= n; ++i)
		result *= i;

	return result;
};

/*! \brief Take the double factorial (n!!) of any type. Similar to Factorial */
template <class Type>
inline Type DoubleFactorial(unsigned int n)
{
	Type result(1);

	for(unsigned int i = n; i > 1; i -= 2)
		result *= i;

	return result;
};

/*! \brief Calculates the binomial coefficient nCr.
 *
 * @param[in] m The upper number in the binomial formula
 * @param[in] n The lower number in the binomial formula
 */
template <class Type>
inline Type BinomialCoefficients(unsigned int m, unsigned int n)
{
	Type result(1);

	if(n < m/2) {
		for(unsigned int i = m - n + 1; i <= m; ++i)
			result *= i;

		for(unsigned int i = 2; i <= n; ++i)
			result /= i;
	} else {
		for(unsigned int i = n + 1; i <= m; ++i)
			result *= i;

		for(unsigned int i = 2; i <= m - n; ++i)
			result /= i;
	}

	return result;
};


/*! \brief Calculates a number modulus another. Only enabled for integer input. */
template <
  class Type,
  typename = EnableIf<std::is_integral<Type> >
>
inline Type mod(const Type & x, const Type & m)
{
	Type ret = x % m;

	if(x < (Type)0)
		ret += m;

	return ret;
};

/*! \brief Subtracts a number from another with a modulus. Only enabled for integer input. */
template <
  class Type,
	typename = EnableIf<std::is_integral<Type> >
>
inline Type periodic_subtract(const Type & num, const Type & subtract, const Type & moduli)
{
	if(subtract <= num)
		return (num - subtract);
	else
		return ( (num + moduli) - subtract);
};

/*! \brief Converts a stream to a string. Uses seekg and read.*/
inline std::string streamToString(std::istream & is)
{
	std::string res_str;

	is.seekg(0, std::ios::end);

  auto stream_size = is.tellg();
	res_str.resize(static_cast<std::size_t>(stream_size));

	is.seekg(0, std::ios::beg);
	is.read(&res_str[0], stream_size);

	return res_str;
};

/*! \brief Reorders a container with respect to a new set of indices.
 *
 * The template type must have an access operator available. The function
 * does no bounds-checking whatsoever.
 * @param[in,out] l The container to be reordered
 * @param[in] indices The index basis to use for the reordering.
 * @param[in] len Length of the container and indices
 */
template <class Type>
inline void order_list(Type & l, int *indices, int len)
{
	Type tmp(std::move(l));
	l.resize(len);

	for(int i=0; i<len; i++)
		l[i] = std::move(tmp[indices[i]]);
}

/*!
 * Behaves similar to std::make_pair, but returns a C++11 array insted. Code
 * borrowed from https://gist.github.com/klmr/2775736
 */
template <typename... T>
inline constexpr auto make_array(T&&... values) ->
	std::array<
	  typename std::decay<
		  typename std::common_type<T...>::type>::type,
	    sizeof...(T)>
{
  return std::array<
    typename std::decay<
	    typename std::common_type<T...>::type
    >::type,
	  sizeof...(T)
  >{std::forward<T>(values)...};
}

/// Function that fills the array config with all possible binomial configurations of length _len
/// at every other step. Meaning at _len=4 it fill all configurations of 0, 2 and 4. The bool even
/// decides whether we start at 0 or 1 when filling configurations with an odd _len. At even = true
/// we start at 1, so for _len=5 we fill 1, 3 and 5, while at even = false we start at 0, filling
/// 0, 2 and 4. This is just a header-function, as it calls the recursive function
/// fillProductConfigurationEvenRec

/// The recursive part of the previous header function. It's inputs are the pointer to the configuration, from where to start, how many configs are
/// left to set, and the configuration length.

/*
 *inline int
 *fillProductConfigurationRec(bool **config, std::size_t from, std::size_t left, std::size_t confID, std::size_t _len)
 *{
 *  std::vector<char> baseConf(_len);
 *  std::copy(std::begin(baseConf), std::end(baseConf), baseConf);
 *
 *  //If there are none left to set, just return
 *  if(left == 0){
 *    return confID;
 *  }
 *
 *  //If there is one left to set, set it at each remaining point with a new ID, and return
 *  if(left == 1){
 *    for(auto i = from; i < _len; ++i){
 *      if(i!=from){
 *        confID++;
 *        std::copy(std::begin(baseConf), std::end(baseConf), config[confID]);
 *      }
 *
 *      config[confID][i] = true;
 *    }
 *
 *  //Else, set the mark at every available position, and call itself recursively with one less mark to set
 *  }else{
 *    for(auto i=from; i<(_len-(left-1)); i++){
 *      if(i!=from){
 *        std::copy(std::begin(baseConf), std::end(baseConf), config[confID]);
 *      }
 *
 *      config[confID][i] = true;
 *
 *      confID = fillProductConfigurationRec(config, i+1, left-1, confID, _len);
 *      confID++;
 *    }
 *
 *    //Subtract one for over-counting
 *    confID--;
 *
 *  }
 *
 *  return confID;
 *};
 */

/*
 *inline void fillProductConfiguration(bool **config, int _len, int configs)
 *{
 *  //Sets the initial configuration to be empty.
 *  for(int i=0; i<configs; i++){
 *    for(int j=0; j<_len; j++){
 *      config[i][j] = false;
 *    }
 *  }
 *
 *  int confID = 0;
 *
 *  for(int i=0; i<=_len; i++){
 *    confID = fillProductConfigurationRec(config, 0, i, confID, _len);
 *    confID++;
 *  }
 *};
 */

/*
 *inline void fillProductConfigurationEven(bool **config, int _len, int configs, bool even)
 *{
 *  //Sets the initial configuration to be empty.
 *  for(int i=0; i<configs; i++){
 *    for(int j=0; j<_len; j++){
 *      config[i][j] = false;
 *    }
 *  }
 *
 *  //confID keeps track of which configuration we are at. Meaning the first index of config[][]
 *  int confID = 0;
 *
 *  int start = even?(_len%2):0;
 *
 *  //Fill config at every other step
 *  for(int i=start; i<=_len; i+=2){
 *    confID = fillProductConfigurationRec(config, 0, i, confID, _len);
 *    confID++;
 *  }
 *};
 */

}; //Namespace Utility

/*! \brief Convenient namespace to use when one wants to do print-debuggng */
namespace Debug_Space{
	using std::cout;
	using std::cerr;
	using std::endl;
};

#endif
