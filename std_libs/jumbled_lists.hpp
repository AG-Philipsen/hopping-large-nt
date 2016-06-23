//Created: 13-08-2014
//Modified: Thu 14 Aug 2014 15:47:49 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef JUMBLED_LISTS_HPP
#define JUMBLED_LISTS_HPP

#include<vector>
#include<iostream>
#include<boost/type_traits/is_same.hpp>
#include"template_structs.hpp"

namespace Utility {

template <typename Container = std::vector<int> >
class JumbledList
{
private: 
  Container & container;
  std::vector<int> order;

  void ResetOrder()
  {
    order.resize(container.size());
    for(int i = 0; i < order.size(); ++i)
      order[i] = i;
  };

public:

  typedef typename Container::value_type value_type;
  typedef typename Container::reference reference;
  typedef typename Container::pointer pointer;

  typedef std::vector<int> Keys;

  typedef std::vector<int>::iterator key_iterator;
  typedef std::vector<int>::const_iterator key_const_iterator;

  class iterator
  {
  private:
    JumbledList * jlist;
    std::vector<int>::iterator current;

  public:
		typedef typename Container::iterator::value_type value_type;
		typedef typename Container::iterator::reference reference;
    typedef typename Container::iterator::pointer pointer;
    typedef typename Container::iterator::difference_type difference_type;
    typedef typename Container::iterator::iterator_category iterator_category;

    iterator() : jlist(nullptr) {};

    iterator(JumbledList * source_list, 
             std::vector<int>::iterator start_it) 
      : jlist(source_list),
        current(start_it) {};

    iterator & operator++()
    {
      ++current;
      return *this;
    };

    iterator & operator--()
    {
      --current;
      return *this;
    };

    iterator operator--(int)
    {
      iterator new_it(*this);
      --current;
      return new_it;
    };

    iterator operator++(int)
    {
      iterator new_it(*this);
      ++current;
      return new_it;
    };

    reference operator*()
    {
      return ContainerIndexedReferenceGetter(jlist->container,*current);
    };

    pointer operator->()
    {
      return ContainerIndexedPointerGetter(jlist->container,*current);
    };

    bool operator<(const iterator & right) const
    {
      return current < right.current;
    };

    bool operator==(const iterator & right) const
    {
      return current == right.current;
    };

    bool operator!=(const iterator & right) const
    {
      return current != right.current;
    };

    void swap(iterator rhs) const
    {
      using std::swap;

      swap(*current,*rhs.current);
    };

  };

  iterator begin()
  {
    return iterator(this, order.begin());
  };

  iterator end()
  {
    return iterator(this, order.end());
  };

  key_iterator key_begin()
  {
    return order.begin();
  };

  key_iterator key_end()
  {
    return order.end();
  };

  void Unjumble()
  {
    ResetOrder();
  };

  JumbledList(Container & cont)
    : container(cont)
  {
    ResetOrder();
  };
};

template <typename Container>
inline void iter_swap(
    typename JumbledList<Container>::iterator l,
    typename JumbledList<Container>::iterator r)
{
  l.swap(r);
};

template <typename Container>
struct IteratorSwapper< JumbledList<Container> >
{
  static void swap(
      typename JumbledList<Container>::iterator it1, 
      typename JumbledList<Container>::iterator it2)
  {
    iter_swap<Container>(it1,it2);
  };
};

template <typename Container>
struct IteratorReverser< JumbledList<Container> >
{
  static void reverse(
      typename JumbledList<Container>::iterator first, 
      typename JumbledList<Container>::iterator last)
  {
    while(true) {
      if(first == last or first == --last)
        return;
      else {
        iter_swap<Container>(first,last);
        ++first;
      }
    }
  };
};

} //Namespace Utiliy

#endif /* JUMBLED_LISTS_HPP */
