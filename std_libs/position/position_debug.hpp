//Created: 09-07-2014
//Modified: Tue 12 Aug 2014 14:11:42 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_DEBUG_HPP
#define POSITION_DEBUG_HPP

#include"position_class.hpp"
#include"position_command.hpp"
#include<iostream>
#include<iterator>

namespace Position {

namespace Command {

class RelPosDebugCommand : public RelPosCommand
{
private:
	container & p_list;

public:

	virtual void Execute(container::iterator ex_it)
	{
		using namespace std;

		for(container::iterator it = p_list.begin(); it != p_list.end(); ++it) {

			if( it == ex_it )
				cout << "d( " << *it << " ) ";
			else
				cout << *it << " ";
		}

		cout << endl;
	};

	RelPosDebugCommand(container & p_list) : p_list(p_list) {};
	~RelPosDebugCommand() {};
};

template <int dim>
class AbsPosDebugCommand : public AbsPosCommand<dim>
{
public:
	virtual void Execute(std::vector< std::array<int,dim> > & vecs)
	{
		using namespace std;

		cout << "{ ";

		for(const std::array<int,dim> & arr : vecs) {
			cout << "[";
			std::copy(arr.begin(), arr.end(), ostream_iterator<int>(cout, ","));
			cout << "] ";
		}

		cout << "}" << endl;
	};

	virtual ~AbsPosDebugCommand() {};
};

}; //Namespace Command

}; //Namespace Position


#endif /* POSITION_DEBUG_HPP */
