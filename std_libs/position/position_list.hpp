//Created: 27-06-2014
//Modified: Tue 14 Oct 2014 12:48:28 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_LIST_HPP
#define POSITION_LIST_HPP

#include"position_class.hpp"
#include"position_container.hpp"

namespace Position {

class List : public container
{
private:
	std::list<pos> & list;

protected:
	class IteratorListImp : public IteratorImp
	{
	private:
		std::list<pos>::iterator it;
	
	public:

		IteratorListImp() : IteratorImp() {};
		IteratorListImp(std::list<pos>::iterator it) :IteratorImp(), it(it) {};

		virtual IteratorImp * clone() const
		{
			IteratorListImp * the_clone = new IteratorListImp(it);
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
			return it == dynamic_cast<const IteratorListImp &>(right).it;
		};

		virtual ~IteratorListImp() {};
};

private:
  virtual std::unique_ptr<container> do_clone() const
  {
    return std::unique_ptr<container>{new List(list)};
  };

public:
	virtual iterator begin()
	{
		return iterator(new IteratorListImp(list.begin()));
	};

	virtual iterator end()
	{
		return iterator(new IteratorListImp(list.end()));
	};

  virtual const_iterator begin() const
  {
    return const_iterator(new IteratorListImp(list.begin()));
  }

  virtual const_iterator end() const
  { 
    return const_iterator(new IteratorListImp(list.end()));
  };

  virtual void push_back(const pos & p)
  {
    list.push_back(p);
  };

  virtual void push_back(pos && p)
  {
    list.push_back(std::move(p));
  };

  virtual void push_back_iterator(iterator it)
  { 
    list.push_back(*it);
  };

  virtual size_type size() const
  {
    return list.size();
  };

  virtual bool empty() const
  {
    return list.empty();
  };

	List(std::list<pos> & list) : container(), list(list)  {};
	virtual ~List() {};
};

} //Namespace Position

#endif /* POSITION_LIST_HPP */
