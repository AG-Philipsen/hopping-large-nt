/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 12:32:42 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef SJT_PERMUTATIONS_HPP
#define SJT_PERMUTATIONS_HPP

#include<vector>
#include<algorithm>
#include<boost/utility.hpp>

namespace Utility {
namespace Permutation{

/// Permutations using the Steinhaus-Johnson-Trotter algorithm
/// This one has unfortunately not been implemented to accept lists containing duplicate entries, as
/// the lexicographic ordering has. I have therefore removed the possibility for one to initialise.
/// using a pre-defined list.
class SJT
{
private:

	enum IndexDirection
  {
		IndexForward  =  1,
		IndexBackward = -1,
		IndexStatic   =  0
	};

  typedef std::pair<int, IndexDirection> SetElement;
  typedef std::vector<SetElement>::iterator iterator;

  bool compare(iterator lhs, iterator rhs)
  {
    return lhs->first < rhs->first;
  }

  void FindFirstMovable()
  {
    auto it = std::find_if_not(std::begin(elements), 
                     std::end(elements), 
                     [](const SetElement & e)
                     {
                       return e.second == IndexStatic;
                     });

    if(it == std::end(elements))
    {
      last_element_reached = true;
      return;
    }

    move_it = it;
    ++it;

    while(it != std::end(elements))
    {
      if(it->second != IndexStatic and compare(move_it,it))
      {
        move_it = it;
      }

      ++it;
    }
  }

  void SwapElements()
  {
    auto swap_with = move_it;

    (move_it->second == IndexForward) ? ++swap_with : --swap_with;

    using std::iter_swap;
    iter_swap(move_it, swap_with);

    move_it = swap_with;
  }

  void UpdateMovedElement()
  {
    if( move_it == std::begin(elements) or boost::next(move_it) == std::end(elements))
    {
      move_it->second = IndexStatic;
      return;
    }

    auto next_it = move_it;
    (move_it->second == IndexForward) ? ++next_it : --next_it;

    if(compare(move_it, next_it))
      move_it->second = IndexStatic;
  }

  void UpdateOtherElements()
  {
    auto it = std::begin(elements);

    while(it != move_it)
    {
      if(compare(move_it, it))
        it->second = IndexForward;

      ++it;
    }

    ++it;

    while(it != std::end(elements))
    {
      if(compare(move_it, it))
        it->second = IndexBackward;

      ++it;
    }
  }

public:
  class const_iterator
  {
  private:
    std::vector<SetElement>::const_iterator it;

  public:
    typedef const int value_type;
    typedef const int & reference;
    typedef const int * pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    const_iterator() = default;
    const_iterator(const const_iterator &) = default;

    explicit const_iterator(std::vector<SetElement>::const_iterator v_it)
      : it{v_it} {};

    const_iterator & operator++()
    {
      ++it;
      return *this;
    }

    const_iterator operator++(int)
    {
      auto previous = *this;
      ++(*this);
      return previous;
    }

    const_iterator & operator--()
    {
      --it;
      return *this;
    }

    const_iterator operator--(int)
    {
      auto previous = *this;
      --(*this);
      return previous;
    }

    reference operator*()
    {
      return it->first;
    }

    pointer operator->()
    {
      return &(it->first);
    }

    const_iterator & operator+=(int n)
    {
      it += n;
      return *this;
    }

    const_iterator & operator-=(int n)
    {
      it -= n;
      return *this;
    }

    reference operator[](int n)
    {
      return it[n].first;
    }

    friend difference_type operator-(const const_iterator & lhs, const const_iterator & rhs)
    {
      return lhs.it - rhs.it;
    }

    bool operator==(const const_iterator & right) const
    {
      return it == right.it;
    }

    bool operator!=(const const_iterator & right) const
    {
      return it != right.it;
    };

    bool operator<(const const_iterator & rhs)
    {
      return it < rhs.it;
    }

    bool operator>(const const_iterator & rhs)
    {
      return it > rhs.it;
    }
    
    bool operator<=(const const_iterator & rhs)
    {
      return it <= rhs.it;
    }
    
    bool operator>=(const const_iterator & rhs)
    {
      return it >= rhs.it;
    }
  };

  const_iterator begin() const
  {
    return const_iterator{std::begin(elements)};
  }

  const_iterator end() const
  {
    return const_iterator{std::end(elements)};
  }

  SJT & operator++()
  {
    FindFirstMovable();

    if(last_element_reached)
      return *this;

    SwapElements();
    UpdateMovedElement();
    UpdateOtherElements();

    return *this;
  }

  const int & operator[](std::size_t index) const
  {
    return elements[index].first;
  }

  const int & at(std::size_t index) const
  {
    return elements.at(index).first;
  }

  bool operator!() const
  {
    return !last_element_reached;
  }

  operator bool() const
  {
    return last_element_reached;
  }

  explicit SJT(std::size_t number_of_elements = 0, int count_from = 0)
    : elements(number_of_elements),
      last_element_reached(number_of_elements == 0)
  {
    if(elements.empty())
      return;

    elements[0] = {count_from, IndexStatic};

    for(int i = 1 ; i < number_of_elements ; ++i)
    {
      elements[i] = {i + count_from, IndexBackward};
    }
  };

private:
  std::vector<SetElement> elements;
  bool last_element_reached;
  
  iterator move_it;
};

inline SJT::const_iterator 
operator+(SJT::const_iterator lhs, int n)
{
  lhs += n;
  return lhs;
}

inline SJT::const_iterator 
operator+(int n, SJT::const_iterator rhs)
{
  rhs += n;
  return rhs;
}

inline SJT::const_iterator 
operator-(SJT::const_iterator lhs, int n)
{
  lhs -= n;
  return lhs;
}

inline SJT::const_iterator 
operator-(int n, SJT::const_iterator rhs)
{
  rhs -= n;
  return rhs;
}

}}

#endif /* SJT_PERMUTATIONS_HPP */
