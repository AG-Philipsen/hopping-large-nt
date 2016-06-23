//Created: 14-07-2014
//Modified: Fri 17 Oct 2014 19:53:39 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef COUNTED_SET_HPP
#define COUNTED_SET_HPP

#include"counted_object.hpp"
#include<set>
#include<vector>
#include<ostream>
#include<stdexcept>

namespace Utility {

/*! \brief A set which keeps count of its objects.
 *
 * Has many similarities with std::set, except that it keeps
 * a count of the objects in the set. It uses the Counted class
 * to achieve this.
 *
 * The class is written as a header only template class.
 */
template <typename Type, typename Compare = std::less< Counted<Type> > >
class CountedSet
{
public:
	typedef typename std::set< Counted<Type>, Compare >::iterator iterator;

  /*! \brief Constant iterator access. */
	const iterator begin() const
	{
		return counted_set.begin();
	};

  /*! \brief Constant iterator access. */
	const iterator end() const
	{
		return counted_set.end();
	};

  /*! \brief Returns true of the set is empty, otherwise, false. */
	const bool empty() const
	{
		return counted_set.empty();
	};

  /*! \brief Returns the number of elements in the set. */
	const std::size_t size() const
	{
		return counted_set.size();
	};

  /*! \brief Clears the set, emptying it. */
	void clear() noexcept
	{
		counted_set.clear();
	};

  /*! \brief Finds a specific element in the set.
   *
   * The function makes use of the fact that any object can be
   * implicitly converted to a counted one, with initial count one.
   */
	iterator find(const Type & val) const
	{
		return counted_set.find(val);
	};

  /*! \brief Insert a new element in the set.
   *
   * If the element already exists, its count will be incremented,
   * otherwise it will have a default count of one. The function returns
   * a pair with an iterator pointing the either the new or the updated
   * element, and a boolean telling whether a new element was inserted or not.
   */
	const std::pair<iterator,bool> insert(const Type & val)
	{
		std::pair<iterator,bool> inserted = counted_set.insert(val);
		if(!inserted.second)
			inserted.first->Increment();

		return inserted;
	};

  /*! \brief Inserts a new element by moving, otherwise works like the copy variant */
	const std::pair<iterator,bool> insert(Type && val)
	{
		std::pair<iterator,bool> inserted = counted_set.insert(std::move(val));
		if(!inserted.second)
			inserted.first->Increment();

		return inserted;
	};

  /*! \brief Inserts a counted element into the set.
   *
   * If the set already contains the new element, the count of
   * that element is incremented by the count of the inserted element.
   */
  const std::pair<iterator,bool> insert(const Counted<Type> & val)
  {
    std::pair<iterator,bool> inserted = counted_set.insert(val);

    if(!inserted.second)
      inserted.first->Increment(val.count());

    return inserted;
  };

  /*! \brief Inserts a counted element by moving, otherwise the same as the copy variant. */
  const std::pair<iterator,bool> insert(Counted<Type> && val)
  {
    std::pair<iterator,bool> inserted = counted_set.insert(std::move(val));

    if(!inserted.second)
      inserted.first->Increment(val.count());

    return inserted;
  };

  /*! \brief Inserts an element the "set-way", ignoring counts entierly. */
  const std::pair<iterator,bool> set_insert(const Counted<Type> & val)
  {
    return counted_set.insert(val);
  };

  /*! \brief Inserts an element, ignoring counts, by moving. */
  const std::pair<iterator,bool> set_insert(Counted<Type> && val)
  {
    return counted_set.insert(std::move(val));
  };

  /*! \brief Remove one count of a given value.
   *
   * Searches the set for the value to remove. If the value
   * isn't in the set, {end(), false} will be returned. If the
   * value exists, its count will be decreased by one. If this
   * makes the count zero, the element will be removed from the set,
   * and {next_it, true} will be returned, else {it_to_element,true} 
   * is the return value.
   *
   * The function uses the implicit conversion to from any type
   * to a counted variant when searching.
   */
	const std::pair<iterator,bool> remove_one(const Type & val)
	{
		iterator find_it = counted_set.find(val);

		if(find_it == end())
			return std::make_pair(end(), false);
		else if(find_it->count() > 1)
			find_it->Decrement();
		else
			find_it = counted_set.erase(find_it);

		return std::make_pair(find_it, true);
	};

  /*! \brief Remove all counts of a given element.
   *
   * Similar to the remove_one function, thhis one searches for
   * a counted object of a given value. If the object isn't found,
   * the set is unchanged and {end(), false} is returned. Else
   * the element is removed from the set and {next_it, true} is
   * returned.
   */
	const std::pair<iterator,bool> remove_all(const Type & val)
	{
		iterator find_it = counted_set.find(val);

		if(find_it == end())
			return std::make_pair(end(), false);

		find_it = counted_set.erase(find_it);
		return std::make_pair(find_it, true);
	};

  /*! \brief Decrements the object at a given iterator.
   *
   * Works similar to remove_one, but takes an iterator instead
   * of a value.
   */
	const iterator erase_one(iterator it)
	{
    if(it == end())
      throw std::out_of_range("Cannot erase end()-iterator");

		if(it->count() > 1)
			it->Decrement();
		else
			it = counted_set.erase(it);

		return it;
	};

  /*! \brief Erases an element from the set.
   *
   * Similar to remove_all, but takes an iterator to
   * an element instead of a value.
   */
	const iterator erase_all(iterator it)
	{
    if(it == end())
      throw std::out_of_range("Cannot erase end()-iterator");

		return counted_set.erase(it);
	};

  /*! \brief Fancy printing of the CountedSet */
	friend std::ostream & operator<<(std::ostream & os, const CountedSet & rhs)
	{
		os << "{" << std::endl;

		for(CountedSet::iterator it = rhs.begin(); it != rhs.end(); ++it)
			os << '\t' << *it << ',' << std::endl;

		os << "};";
		return os;
	};

  /*!
   * Compares two sets with the default less than operator, meaning
   * that counts are ignored when comparing.
   */
	friend bool operator<(const CountedSet & lhs, const CountedSet & rhs)
	{
		return lhs.counted_set < rhs.counted_set;
	};

  /*!
   * Returns a vector of the Counted objects, and clears the set, using
   * move semantics for efficiency.
   */
	std::vector< Counted<Type> > extract_data()
	{
		std::vector< Counted<Type> > data;
		data.reserve(counted_set.size());

		for(auto it = counted_set.begin(); it != counted_set.end();) {
			data.push_back( std::move( const_cast<Counted<Type> &>(*it) ) );
			counted_set.erase(it++);
		}

		return data;
	};

private:
	std::set< Counted<Type>, Compare > counted_set;
};

/*! \brief Strict comparison of the elements in a CountedSet.
 *
 * Uses the CountedFullCompare to compare the elements of the set. Thus,
 * the counts are included in the comparison.
 */
template <typename Type, typename Compare = std::less< Counted<Type> > >
struct CountedSetFullCompare
{
  /*! \bried Full less than operator for the two sets. */
  bool operator()(const CountedSet<Type,Compare> & lhs,
                  const CountedSet<Type,Compare> & rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end(),
                                        CountedFullCompare<Type>());
  };
};


} //Namespace Utility

#endif /* COUNTED_SET_HPP */
