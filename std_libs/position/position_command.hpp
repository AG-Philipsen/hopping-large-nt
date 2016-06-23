//Created: 30-06-2014
//Modified: Tue 07 Apr 2015 13:56:19 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_COMMAND_HPP
#define POSITION_COMMAND_HPP

#include"position_container.hpp"
#include<vector>
#include<array>

namespace Position {

class pos;

namespace Command {

class RelPosCommand
{
public:
	virtual void Execute(container::iterator it) = 0;
	virtual ~RelPosCommand() {};
};

template <std::size_t dim>
class AbsPosCommand
{
public:

  typedef std::vector< std::array<int,dim> > value_type;

	virtual void Execute(value_type &) = 0;
	virtual ~AbsPosCommand() {};
};

} //Namespace Command

} //Namespace Position


#endif /* POSITION_COMMAND_HPP */
