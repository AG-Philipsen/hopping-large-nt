/*
 * Created: 14-10-2014
 * Modified: Wed 15 Oct 2014 11:04:17 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#ifndef TEST_COMMANDS_HPP
#define TEST_COMMANDS_HPP

#include<position/position_command.hpp>
#include<position/position_class.hpp>
#include<counted_set.hpp>
#include<list>

namespace Position {
namespace Command {

class TestRelPosCommand : public RelPosCommand
{
public:
  
  typedef std::list<pos> PosContainer;
  typedef std::list<PosContainer> ResultFormat;

	virtual void Execute(container::iterator it);

  TestRelPosCommand(const PosContainer & base, const ResultFormat & expected)
    : base_list(base),
      expected_result(expected),
      result_pos(expected_result.begin())
  {};

private:
  const PosContainer & base_list;
  const ResultFormat & expected_result;
  ResultFormat::const_iterator result_pos;
};

template <int dim>
class TestSimpleTermCollector : public AbsPosCommand<dim>
{
public:
  typedef typename AbsPosCommand<dim>::value_type value_type;
  typedef std::list<value_type> container_type;

	virtual void Execute(value_type & term)
  {
    all_terms.push_back(term);
  }

  container_type ExtractData()
  {
    return std::move(all_terms);
  }

  virtual ~TestSimpleTermCollector() {};

private:
  container_type all_terms;
};

template <int dim>
class TestAdvancedTermCollector : public AbsPosCommand<dim>
{
public:
  typedef typename AbsPosCommand<dim>::value_type value_type;
  typedef Utility::CountedSet<value_type> container_type;

  virtual void Execute(value_type & term)
  {
    all_terms.insert(term);
  }

  container_type ExtractData()
  {
    return std::move(all_terms);
  }

  virtual ~TestAdvancedTermCollector() {};

private:
  container_type all_terms;
};

} //Namespace Command
} //Namespace Position


#endif /* TEST_COMMANDS_HPP */
