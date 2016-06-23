/*
 * Created: 25-03-2015
 * Modified: Mon 13 Apr 2015 15:58:05 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef MATH_SUM_HPP
#define MATH_SUM_HPP

#include"math_term.hpp"

namespace Math {

template <
  typename Type, 
  typename Numeric = long long, 
  typename Comparator = std::less<Type>, 
  bool Abelian = true>
class Sum
{
private:

  typedef Term<Type,Numeric,Comparator,Abelian> SumTerm;
  std::vector<SumTerm> terms;

  typedef typename std::vector<SumTerm>::iterator iterator;

public:

  typedef std::vector<SumTerm> containter_type;
  typedef typename containter_type::const_iterator const_iterator;

  const_iterator begin() const
  {
    return terms.begin();
  };

  const_iterator end() const
  {
    return terms.end();
  };

  const std::size_t size() const
  {
    return terms.size();
  };

  const bool empty() const
  {
    return terms.empty();
  };

  Sum & operator+=(SumTerm rhs)
  {
    SortTerm(rhs);

    iterator element_it = std::lower_bound(terms.begin(), terms.end(), rhs);

    if( !(element_it == terms.end()) and !(rhs < *element_it) )
      element_it->prefactor_ref() += rhs.prefactor();
    else
      terms.insert(element_it, std::move(rhs));

    return *this;
  }

  Sum & operator+=(Sum rhs)
  {
    for(SumTerm & term : rhs.terms)
      operator+=(std::move(term));

    return *this;
  };

  Sum & operator-=(SumTerm rhs)
  {
    rhs *= Numeric{-1};
    return operator+=(std::move(rhs));
  }

  SumTerm extract(const_iterator it)
  {
    SumTerm extracted_term(std::move(const_cast<SumTerm &>(*it)));
    terms.erase(it);

    return extracted_term;
  };

  friend std::ostream & operator<<(std::ostream & os, const Sum & s)
  {
    for(const auto & t : s)
      os << t << std::endl;

    return os;
  };
};

template <
  typename Type,
  typename Numeric = long long,
  typename Comparator = std::less<Type> >
using AbelianSum = Sum<Type, Numeric, Comparator, true>;


template <
  typename Type,
  typename Numeric = long long,
  typename Comparator = std::less<Type> >
using NonAbelianSum = Sum<Type, Numeric, Comparator, false>;

template <
  typename Type, 
  typename Numeric, 
  typename Comparator, 
  bool Abelian>
void SortTerm( Term<Type,Numeric,Comparator,Abelian> &);

template <
  typename Type, 
  typename Numeric, 
  typename Comparator>
inline void SortTerm( Term<Type,Numeric,Comparator,true> & term)
{
  term.sort();
};

template <
  typename Type, 
  typename Numeric, 
  typename Comparator>
inline void SortTerm( Term<Type,Numeric,Comparator,false> & term)
{
};

} //namespace Math


#endif /* MATH_SUM_HPP */
