//Created: 10-02-2014
//Modified: Wed 25 Feb 2015 11:47:28 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_CLASS_H
#define POSITION_CLASS_H

#include<deque>
#include<initializer_list>

#include<algorithm>
#include<utility>
#include<fstream>

#include<stdexcept>

#include"../std_funcs.h"
#include"../error.h"

namespace Position {

namespace Manipulator {
class CoordinateCleaner;
}

class pos
{
friend class Manipulator::CoordinateCleaner;
friend class Parser;

private:
	std::deque<int> pos_vec;

public:

  typedef int value_type;
  typedef std::deque<int>::size_type size_type;
  typedef int& reference;
  typedef int* pointer;
  typedef const int& const_reference;
  typedef const int* const_pointer;
  typedef std::deque<int>::iterator iterator;
  typedef std::deque<int>::const_iterator const_iterator;

	pos() {};
	pos(size_type l) : pos_vec(l,0) {};
	pos(const pos &v) = default;
	pos(pos &&v) = default;

	template <
    class InputIterator,
		typename = EnableIf< 
      std::is_same<
        typename std::iterator_traits<InputIterator>::value_type, 
        value_type>
    > 
  >
	pos(InputIterator begin, InputIterator end) : pos_vec(begin,end) {};

	pos(std::initializer_list<int> l) : pos_vec(l) {};
  
	pos& operator=(const pos &v) = default;
	pos& operator=(pos &&v) = default;

	reference operator[](size_type index)
	{
		if(index < pos_vec.size()) {
			return pos_vec[index];
		} else {
			pos_vec.resize(index + 1, 0);
			return pos_vec[index];
		}
	};

  reference at(size_type index)
  {
    return pos_vec.at(index);
  };

  value_type at(size_type index) const
  {
    return pos_vec.at(index);
  };

  size_type size() const
	{
		return pos_vec.size();
	};

  iterator begin()
  {
    return pos_vec.begin();
  }

  iterator end()
  {
    return pos_vec.end();
  }

  const_iterator begin() const
  {
    return pos_vec.begin();
  }

  const_iterator end() const
  {
    return pos_vec.end();
  }

  void resize_back(size_type new_size)
  {
    pos_vec.resize(new_size,0);
  };

  void resize_front(size_type new_size)
  {
    auto initial_size = pos_vec.size();

    if(new_size < initial_size){
      for(int i = 0; i < (initial_size - new_size); ++i)
        pos_vec.pop_front();
    }else{
      for(int i = 0; i < (new_size - initial_size); ++i)
        pos_vec.push_front(0);
    }
  };

	pos& operator+= (int i) 
	{ 
    if(i == 0)
      throw std::logic_error("Cannot add zero to a pos class");

		int index = abs(i) - 1;
		int len = pos_vec.size();

		if(index < len) 
			pos_vec[index] += Utility::sign(i);
		else {
			pos_vec.resize(index + 1, 0);
			pos_vec[index] += Utility::sign(i);
		}

		return *this; 
	};
	 
	pos& operator-= (int i) 
	{ 
    *this += -i;
    return *this;
	};

	pos& operator+= (const pos &rhs)
	{
		int llen = pos_vec.size(), rlen = rhs.pos_vec.size();

		if(llen < rlen) {

			for(int i = 0; i < llen; ++i)
				pos_vec[i] += rhs.pos_vec[i];

			for(int i = llen; i < rlen; ++i)
				pos_vec.push_back(rhs.pos_vec[i]);
		} else {
			
			for(int i = 0; i < rlen; ++i)
				pos_vec[i] += rhs.pos_vec[i];
		}

		return *this;
	};

	pos& operator-= (const pos &rhs)
	{
		int llen = pos_vec.size(), rlen = rhs.pos_vec.size();

		if(llen < rlen) {

			for(int i = 0; i < llen; ++i)
				pos_vec[i] -= rhs.pos_vec[i];

			for(int i = llen; i < rlen; ++i)
				pos_vec.push_back(-rhs.pos_vec[i]);
		} else {
			
			for(int i = 0; i < rlen; ++i)
				pos_vec[i] -= rhs.pos_vec[i];
		}

		return *this;
	};

	const pos operator-() const
	{
		pos result(*this);

		for(auto &index : result.pos_vec)
			index = -index;

		return result;
	};

	bool operator==(const pos &v) const 
	{ 
		const std::deque<int> *long_vec, *short_vec;

		if(pos_vec.size() < v.pos_vec.size()) {
			short_vec = &pos_vec;
			long_vec = &v.pos_vec;
		} else {
			short_vec = &v.pos_vec;
			long_vec = &pos_vec;
		}

		for(int i = 0; i < short_vec->size(); ++i) {
			if( (*short_vec)[i] != (*long_vec)[i] )
				return false;
		}

		for(int i = short_vec->size(); i < long_vec->size(); ++i) {
			if( (*long_vec)[i] != 0 )
				return false;
		}

		return true;
	};

	bool operator!=(const pos &v) const 
	{ 
		return !(operator==(v));
	};

	bool operator<(const pos &v) const
	{
		int llen = pos_vec.size(), rlen = v.pos_vec.size();

		if(llen < rlen) {
			for(int i = 0; i < llen; ++i) {
				if(pos_vec[i] != v.pos_vec[i]) 
					return (pos_vec[i] < v.pos_vec[i]);
			}

			for(int i = llen; i < rlen; ++i) {
				if(0 != v.pos_vec[i]) 
					return ( 0 < v.pos_vec[i]);
			}
		} else {
			for(int i = 0; i < rlen; ++i) {
				if(pos_vec[i] != v.pos_vec[i]) 
					return (pos_vec[i] < v.pos_vec[i]);
			}

			for(int i = rlen; i < llen; ++i) {
				if(0 != pos_vec[i]) 
					return ( pos_vec[i] < 0 );
			}
		}
		
		return false;
	};

	friend std::ostream& operator<<(std::ostream &os, const pos &v);

	bool is_zero() const 
	{ 
		return std::all_of(pos_vec.begin(), pos_vec.end(), 
				[](int x){
					return x == 0;
				}); 
	};

  bool is_empty() const
  {
    return pos_vec.empty();
  };

	void deleteTrailingZeros()
	{
		auto reverse_it = std::find_if(
        pos_vec.rbegin(), 
        pos_vec.rend(), 
        [](int i) {return i != 0;} );

		pos_vec.erase(reverse_it.base(),pos_vec.end());
	};
};

inline const pos operator+ (pos lhs, const pos & rhs)
{
  lhs += rhs;
  return lhs;
}

inline const pos operator- (pos lhs, const pos & rhs)
{
  lhs -= rhs;
  return lhs;
}

inline const pos operator+ (pos lhs, int index)
{
  lhs += index;
  return lhs;
}

inline const pos operator- (pos lhs, int index)
{
  lhs -= index;
  return lhs;
}

inline std::ostream& operator<<(std::ostream &os, const pos &v)
{

	os << "{";
	
	if(v.pos_vec.size() == 0){
		os << "}";
		return os;
	}

	os << v.pos_vec[0];

	for(int i=1; i<v.pos_vec.size(); i++){
		os << "," << v.pos_vec[i];
	}

	os << "}";

	return os;
}

}


#endif /* POSITION_CLASS_H */
