//Created: 01-07-2014
//Modified: Tue 14 Oct 2014 12:51:51 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_VECTOR_HPP
#define POSITION_VECTOR_HPP

#include"position_class.hpp"
#include"position_container.hpp"
#include<vector>

namespace Position {

class Vector : public container
{
private:
	std::vector<pos> & vec;

protected:
	class IteratorVectorImp : public IteratorImp
	{
	private:
		std::vector<pos>::iterator it;

	public:

		IteratorVectorImp() : IteratorImp() {};
		IteratorVectorImp(std::vector<pos>::iterator it) : IteratorImp(), it(it)  {};

		virtual IteratorImp * clone() const
		{
			IteratorVectorImp * the_clone = new IteratorVectorImp(it);
			return the_clone;
		};

		virtual void increment()
		{
			++it;
		};

		virtual pos & value()
		{
			return *it;
		};

		virtual const pos & value() const
		{
			return *it;
		};

		virtual bool equal(const IteratorImp & right) const
		{
			return it == dynamic_cast<const IteratorVectorImp &>(right).it;
		};

		virtual ~IteratorVectorImp() {};
	};

private:
  virtual std::unique_ptr<container> do_clone() const
  {
    return std::unique_ptr<container>{new Vector(vec)};
  };

public:
	virtual iterator begin()
	{
		return iterator(new IteratorVectorImp(vec.begin()));
	};

	virtual iterator end()
	{
		return iterator(new IteratorVectorImp(vec.end()));
	};

  virtual const_iterator begin() const
  {
    return const_iterator(new IteratorVectorImp(vec.begin()));
  };

  virtual const_iterator end() const
  {
    return const_iterator(new IteratorVectorImp(vec.end()));
  };

	pos & operator[](size_type i)
	{
		return vec[i];
	};

	pos & at(size_type i)
	{
		return vec.at(i);
	};

  virtual void push_back(const pos & p)
  {
    vec.push_back(p);
  };

  virtual void push_back(pos && p)
  {
    vec.push_back(std::move(p));
  };

  virtual void push_back_iterator(iterator it)
  {
    vec.push_back(*it);
  };

  virtual size_type size() const
  {
    return vec.size();
  };

  virtual bool empty() const
  {
    return vec.empty();
  };

	Vector(std::vector<pos> & vec) : container(), vec(vec)  {};
	virtual ~Vector() {};
};

} //Namespace Position

#endif /* POSITION_VECTOR_HPP */
