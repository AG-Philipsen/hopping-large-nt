//Created: 30-06-2014
//Modified: Wed 10 Dec 2014 10:49:24 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_WILSON_VECTOR_HPP
#define POSITION_WILSON_VECTOR_HPP

#include"std_libs/position/position_container.hpp"
#include"pm.wilson.h"

class PositionWilsonVector : public Position::container
{
private:
  std::vector<hop::Wilson> & wvec;
  
  class IteratorWilsonImp : public IteratorImp
  {
  private:
    std::vector<hop::Wilson>::iterator it;

  public:
    IteratorWilsonImp() : IteratorImp() {};
    IteratorWilsonImp(std::vector<hop::Wilson>::iterator it) : it(it) {};

    virtual IteratorImp * clone() const
    {
      IteratorWilsonImp * the_clone = new IteratorWilsonImp(it);
      return the_clone;
    };

    virtual void increment()
    {
      ++it;
    };

    virtual Position::pos & value()
    {
      return it->pos;
    };

    virtual const Position::pos & value() const
    {
      return it->pos;
    };

    virtual bool equal(const IteratorImp & right) const
    {
      return it == dynamic_cast<const IteratorWilsonImp &>(right).it;
    };

    virtual ~IteratorWilsonImp() {};
  };

private:
  virtual std::unique_ptr<container> do_clone() const
  {
    return std::unique_ptr<container> {new PositionWilsonVector(wvec)};
  };

public:
  virtual iterator begin()
  {
    return iterator( new IteratorWilsonImp(wvec.begin()) );
  };

  virtual iterator end()
  {
    return iterator( new IteratorWilsonImp(wvec.end()) );
  };

  virtual const_iterator begin() const
  {
    return const_iterator( new IteratorWilsonImp(wvec.begin()) );
  };

  virtual const_iterator end() const
  {
    return const_iterator( new IteratorWilsonImp(wvec.end()) );
  };

  PositionWilsonVector(std::vector<hop::Wilson> & wvec) : wvec(wvec) {};
  virtual ~PositionWilsonVector() {};
};

#endif /* POSITION_WILSON_VECTOR_HPP */
