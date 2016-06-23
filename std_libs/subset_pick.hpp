/*
 * Created: 02-10-2014
 * Modified: Tue 14 Oct 2014 10:40:54 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef PICK_FROM_HPP
#define PICK_FROM_HPP

#include "typedefs.hpp"
#include<iterator>
#include<stdexcept>
#include<vector>
#include<algorithm>

namespace Utility {

template <
  typename InputIt,
  typename = DisableIf< std::is_same<typename std::iterator_traits<InputIt>::value_type, void> >
>
class SubsetPick
{
private:
  
  template <bool skip_value>
  class skip_iterator
  {
  private:
    InputIt element_iterator;
    std::vector<char>::iterator lex_iterator, lex_end;

  public:
    typedef typename InputIt::value_type value_type;
    typedef typename InputIt::reference reference;
    typedef typename InputIt::pointer pointer;
    typedef typename InputIt::difference_type difference_type;
    typedef std::forward_iterator_tag iterator_category;

    skip_iterator & operator++()
    {
      do
      {
        ++element_iterator;
        ++lex_iterator;
      }while( lex_iterator != lex_end and *lex_iterator == skip_value );

      return *this;
    }

    skip_iterator operator++(int)
    {
      auto result = *this;
      ++(*this);
      return result;
    }

    reference operator*() const
    {
      return *element_iterator;
    }

    pointer operator->() const
    {
      return &(*element_iterator);
    }

    bool operator==(const skip_iterator & rhs) const
    {
      return ( element_iterator == rhs.element_iterator and lex_iterator == rhs.lex_iterator );
    }

    bool operator!=(const skip_iterator & rhs) const
    {
      return !(operator==(rhs));
    }

    skip_iterator() = default;
    skip_iterator(const skip_iterator &) = default;

    skip_iterator(InputIt it, std::vector<char>::iterator p_it, std::vector<char>::iterator p_end)
      : element_iterator{it},
        lex_iterator{p_it},
        lex_end{p_end}
    {
      while(lex_iterator != lex_end and *lex_iterator == skip_value)
      {
        ++element_iterator;
        ++lex_iterator;
      }
    };
  };

  template <bool skip_value>
  class const_skip_iterator
  {
  private:
    InputIt element_iterator;
    std::vector<char>::const_iterator lex_iterator, lex_end;

  public:
    typedef const typename InputIt::value_type value_type;
    typedef const typename InputIt::reference reference;
    typedef const typename InputIt::pointer pointer;
    typedef typename InputIt::difference_type difference_type;
    typedef std::forward_iterator_tag iterator_category;

    const_skip_iterator & operator++()
    {
      do
      {
        ++element_iterator;
        ++lex_iterator;
      }while( lex_iterator != lex_end and *lex_iterator == skip_value );

      return *this;
    }

    const_skip_iterator operator++(int)
    {
      auto result = *this;
      ++(*this);
      return result;
    }

    reference operator*() const
    {
      return *element_iterator;
    }

    pointer operator->() const
    {
      return &(*element_iterator);
    }

    bool operator==(const const_skip_iterator & rhs) const
    {
      return ( element_iterator == rhs.element_iterator and lex_iterator == rhs.lex_iterator );
    }

    bool operator!=(const const_skip_iterator & rhs) const
    {
      return !(operator==(rhs));
    }

    const_skip_iterator() = default;
    const_skip_iterator(const const_skip_iterator &) = default;

    const_skip_iterator(InputIt it, std::vector<char>::const_iterator p_it, std::vector<char>::const_iterator p_end)
      : element_iterator{it},
        lex_iterator{p_it},
        lex_end{p_end}
    {
      while(lex_iterator != lex_end and *lex_iterator == skip_value)
      {
        ++element_iterator;
        ++lex_iterator;
      }
    };
  };

public:

  using pick_iterator = skip_iterator<false>;
  using rest_iterator = skip_iterator<true>;

  using const_pick_iterator = const_skip_iterator<false>;
  using const_rest_iterator = const_skip_iterator<true>;

  pick_iterator begin()
  {
    return {first, std::begin(in_current_pick), std::end(in_current_pick)};
  }

  pick_iterator end()
  {
    return {last, std::end(in_current_pick), std::end(in_current_pick)};
  }

  const_pick_iterator begin() const
  {
    return {first, in_current_pick.cbegin(), in_current_pick.cend()};
  }

  const_pick_iterator end() const
  {
    return {last, in_current_pick.cend(), in_current_pick.cend()};
  }

  pick_iterator pick_begin()
  {
    return begin();
  }

  pick_iterator pick_end()
  {
    return end();
  }

  const_pick_iterator pick_begin() const
  {
    return begin();
  }

  const_pick_iterator pick_end() const
  {
    return end();
  }

  rest_iterator rest_begin()
  {
    return {first, std::begin(in_current_pick), std::end(in_current_pick)};
  }

  rest_iterator rest_end()
  {
    return {last, std::end(in_current_pick), std::end(in_current_pick)};
  }

  const_rest_iterator rest_begin() const
  {
    return {first, std::begin(in_current_pick), std::end(in_current_pick)};
  }

  const_rest_iterator rest_end() const
  {
    return {last, std::end(in_current_pick), std::end(in_current_pick)};
  }

  bool operator!() const
  {
    return !last_pick_reached;
  }

  operator bool() const
  {
    return last_pick_reached;
  }

  SubsetPick & operator++()
  {
    if(last_pick_reached)
      return *this;

    if(!std::next_permutation(std::begin(in_current_pick), std::end(in_current_pick), pick_compare))
    {
      last_pick_reached = true;
    }

    return *this;
  }

  SubsetPick(InputIt first_it, InputIt last_it, std::size_t pick_elements)
    : first{first_it},
      last{last_it},
      in_current_pick(std::distance(first_it, last_it), false),
      last_pick_reached(false)
  {
    auto len = std::distance(first, last);

    if(pick_elements > len)
      throw std::logic_error("Cannot pick more elements than there are");

    if(len == 0 or pick_elements == 0)
      last_pick_reached = true;

    for(std::size_t i = 0;  i < pick_elements; ++i)
      in_current_pick[i] = true;
  };

private:
  InputIt first, last;
  std::vector<char> in_current_pick;
  std::greater<bool> pick_compare;

  bool last_pick_reached;
};

}

#endif /* PICK_FROM_HPP */
