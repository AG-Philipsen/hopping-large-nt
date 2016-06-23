//Created: 27-06-2014
//Modified: Tue 14 Oct 2014 12:45:32 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_CONTAINER_HPP
#define POSITION_CONTAINER_HPP

#include<cstddef>
#include<iterator>
#include<memory>
#include<boost/utility.hpp>

namespace Position {

class pos;

class container : boost::noncopyable
{
protected:
	class IteratorImp
	{
	public:
		virtual IteratorImp * clone() const = 0;
		virtual void increment() = 0;
		virtual pos & value() = 0;
    virtual const pos & value() const = 0;
		virtual bool equal(const IteratorImp & right) const = 0;

		virtual ~IteratorImp() {};
	};

private:
  virtual std::unique_ptr<container> do_clone() const = 0;

public:

  typedef pos value_type;
  typedef pos& reference;
  typedef const pos& const_reference;
  typedef pos* pointer;
  typedef const pos* const_pointer;
  typedef std::size_t size_type;

	class iterator
	{
	private:
    std::unique_ptr<IteratorImp> impl;
	
	public:
		typedef pos value_type;
		typedef pos& reference;
		typedef pos* pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::forward_iterator_tag iterator_category;

		iterator() : impl() {};
		iterator(IteratorImp * impl) : impl(impl) {};
		iterator(const iterator & right) : impl(right.impl->clone()) {};

		~iterator() {};

    const IteratorImp * GetImplementation() const
    {
      return impl.get();
    };

		iterator & operator=(const iterator & right)
		{
      impl.reset(right.impl->clone());
			return *this;
		};

		iterator & operator++()
		{
			impl->increment();
			return *this;
		};

		iterator operator++(int)
		{
			iterator current(*this);
			impl->increment();
			return current;
		};

		pos & operator*()
		{
			return impl->value();
		};

		pos * operator->()
		{
			return &(impl->value());
		};

		bool operator==(const iterator & right) const
		{
			return impl->equal(*(right.impl));
		};

		bool operator!=(const iterator & right) const
		{
			return !( impl->equal(*(right.impl)) );
		};
	};

  class const_iterator
	{
	private:
    std::unique_ptr<IteratorImp> impl;
	
	public:
		typedef pos value_type;
		typedef const pos & reference;
		typedef const pos * pointer;
		typedef std::ptrdiff_t difference_type;
		typedef std::forward_iterator_tag iterator_category;

		const_iterator() : impl() {};
		const_iterator(IteratorImp * impl) : impl(impl) {};
		const_iterator(const const_iterator & right) : impl(right.impl->clone()) {};

    const IteratorImp * GetImplementation() const
    {
      return impl.get();
    };

		const_iterator & operator=(const const_iterator & right)
		{
      impl.reset(right.impl->clone());
			return *this;
		};

		const_iterator & operator++()
		{
			impl->increment();
			return *this;
		};

		const_iterator operator++(int)
		{
			const_iterator current(*this);
			impl->increment();
			return current;
		};

		const pos & operator*()
		{
			return impl->value();
		};

		const pos * operator->()
		{
			return &(impl->value());
		};

		bool operator==(const const_iterator & right) const
		{
			return impl->equal(*(right.impl));
		};

		bool operator!=(const const_iterator & right) const
		{
			return !( impl->equal(*(right.impl)) );
		};
	};

	virtual iterator begin() = 0;
	virtual iterator end() = 0;

  virtual const_iterator begin() const = 0;
  virtual const_iterator end() const = 0;

  virtual void push_back(const pos & p)
  {
    throw std::runtime_error("The container haven't implemented push_back(pos)");
  };

  virtual void push_back(pos && p)
  {
    throw std::runtime_error("The container haven't implemented push_back(pos&&)");
  };

  virtual void push_back_iterator(iterator)
  {
    throw std::runtime_error("The container haven't implemented push_back_iterator(iterator)");
  };

  virtual void merge_iterator(iterator,iterator)
  {
    throw std::runtime_error("The container haven't implemented merge_iterator(iterator,iterator)");
  };

  virtual size_type size() const
  {
    return std::distance(begin(), end());
  };

  virtual bool empty() const
  {
    return size() == 0;
  };

  std::unique_ptr<container> clone() const
  {
    return do_clone();
  };

	virtual ~container() {};
};

inline container * new_clone(const container & c)
{
  auto the_clone = c.clone();
  return the_clone.release();
};

}

#endif /* POSITION_CONTAINER_HPP */
