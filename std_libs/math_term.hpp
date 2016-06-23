//Created: 23-07-2014
//Modified: Mon 13 Apr 2015 15:57:55 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef MATH_TERM_HPP
#define MATH_TERM_HPP

#include<vector>
#include<ostream>
#include<stdexcept>
#include<algorithm>

#include"typedefs.hpp"

namespace Math {

template <
  typename Type, 
  typename Numeric = long long, 
  typename Comparator = std::less<Type>, 
  bool Abelian = true>
class Term
{
private:
  Numeric pref;
  std::vector<Type> facs;

  Comparator compare;

public:
  Numeric prefactor() const
  {
    return pref;
  };

  Numeric & prefactor_ref()
  {
    return pref;
  };

  typedef std::vector<Type> containter_type;
  typedef Numeric numeric_type;

  typedef typename containter_type::iterator iterator;
  typedef typename containter_type::const_iterator const_iterator;

  Type & operator[] (std::size_t index)
  {
    return facs[index];
  }

  const Type & operator[] (std::size_t index) const
  {
    return facs[index];
  }

  iterator begin()
  {
    return facs.begin();
  };

  iterator end()
  {
    return facs.end();
  };

  const_iterator begin() const
  {
    return facs.begin();
  };

  const_iterator end() const
  {
    return facs.end();
  };

  const bool empty() const
  {
    return facs.empty();
  };

  const std::size_t size() const
  {
    return facs.size();
  };

  Term & operator*=(const Type & rhs)
  {
    facs.push_back(rhs);
    return *this;
  };

  Term & operator*=(Type && rhs)
  {
    facs.push_back(std::move(rhs));
    return *this;
  };

  Term & operator*=(Numeric rhs)
  {
    pref *= rhs;
    return *this;
  };

  Term & operator*=(Term rhs)
  {
    pref *= rhs.pref;
    facs.insert(std::end(facs),
                std::make_move_iterator(std::begin(rhs)),
                std::make_move_iterator(std::end(rhs)));
    return *this;
  };

  friend Term operator*(Term lhs, Term rhs)
  {
    lhs *= std::move(rhs);
    return lhs;
  };

  friend Term operator*(Term lhs, const Type & rhs)
  {
    lhs *= rhs;
    return lhs;
  };

  friend Term operator*(const Type & lhs, Term rhs)
  {
    rhs.facs.insert(std::begin(rhs.facs), lhs);
    return rhs;
  };

  friend Term operator*(Term lhs, Numeric rhs)
  {
    lhs *= rhs;
    return lhs;
  };

  friend Term operator*(Numeric lhs, Term rhs)
  {
    rhs *= lhs;
    return rhs;
  };

  void sort()
  {
    if(!Abelian)
      throw std::logic_error("Cannot sort a Non-Abelian set of terms");

    std::sort(std::begin(facs), std::end(facs), compare);
  };

  bool operator<(const Term & rhs) const
  {
    return std::lexicographical_compare(
        facs.begin(), facs.end(),
        rhs.facs.begin(), rhs.facs.end(),
        compare);
  };

  friend std::ostream & operator<<(std::ostream & os, const Term & t)
  {
    os << "(" << t.pref << ")";
    for(const Type & f : t)
     os << " . "  << f;

    return os;

  };

  Term() : pref(1) {};
  explicit Term(const Type & fac) : pref(1), facs(1,fac) {};
  explicit Term(Type && fac) : pref(1), facs(1,std::move(fac)) {};

  explicit Term(const Numeric & pref) : pref(pref) {};
};

template <typename Type,
          typename Numeric = long long,
          typename Comparator = std::less<Type> >
using AbelianTerm = Term<Type, Numeric, Comparator, true>;

template <typename Type,
          typename Numeric = long long,
          typename Comparator = std::less<Type> >
using NonAbelianTerm = Term<Type, Numeric, Comparator, false>;

}

#endif /* MATH_TERM_HPP */
