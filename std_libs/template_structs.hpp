//Created: 14-08-2014
//Modified: Tue 30 Sep 2014 11:28:38 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef TEMPLATE_STRUCTS_HPP
#define TEMPLATE_STRUCTS_HPP

#include<algorithm>
#include<boost/type_traits/is_same.hpp>

namespace Utility {

template <bool random_access>
struct ContainerElementGetterImpl;

template <>
struct ContainerElementGetterImpl<true>
{
  template <typename Container>
  static typename Container::iterator::reference
  GetReference(Container & container, int key)
  {
    return *(container.begin() + key);
  };

  template <typename Container>
  static typename Container::iterator::pointer
  GetPointer(Container & container, int key)
  {
    return &(GetReference(container,key));
  };
};

template <>
struct ContainerElementGetterImpl<false>
{
  template <typename Container>
  static typename Container::iterator::reference
  GetReference(Container & container, int key)
  {
    auto it = container.begin();
    std::advance(it, key);
    return *it;
  };

  template <typename Container>
  static typename Container::iterator::pointer
  GetPointer(Container & container, int key)
  {
    return &(GetReference(container,key));
  };
};

template <typename Container>
inline typename Container::iterator::reference
ContainerIndexedReferenceGetter(Container & container, int index)
{
  const bool impl_type =
    boost::is_same<
      typename Container::iterator::iterator_category,
      std::random_access_iterator_tag
    >::value;

  return ContainerElementGetterImpl<impl_type>::GetReference(container,index);
};

template <typename Container>
inline typename Container::iterator::pointer
ContainerIndexedPointerGetter(Container & container, int index)
{
  const bool impl_type =
    boost::is_same<
      typename Container::iterator::iterator_category,
      std::random_access_iterator_tag
    >::value;

  return ContainerElementGetterImpl<impl_type>::GetPointer(container,index);
};

template <typename Container>
struct IteratorSwapper
{
  static void swap(
      typename Container::iterator it1, 
      typename Container::iterator it2)
  {
    using std::iter_swap;
    iter_swap(it1,it2);
  };
};

template <typename Container>
struct IteratorReverser
{
  static void reverse(
      typename Container::iterator first,
      typename Container::iterator last)
  {
    using std::reverse;
    reverse(first,last);
  };
};

} //Namespace Utility

#endif /* TEMPLATE_STRUCTS_HPP */
