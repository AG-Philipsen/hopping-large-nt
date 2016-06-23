//Created: 25-06-2014
//Modified: Tue 07 Apr 2015 13:56:28 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_MANIP_HPP
#define POSITION_MANIP_HPP

#include"position_class.hpp"
#include"../typedefs.hpp"
#include"position_command.hpp"
#include"position_container.hpp"

#include<set>

namespace Position {
namespace Manipulator {

class CoordinateCleaner
{
private:
	std::set<int> used_indices;
	std::set<int> redundant_indices;

	void fillUsedIndices(const pos & p)
	{
		size_t len = p.pos_vec.size();

		for(int i = 0; i < len; ++i)
			if(p.pos_vec[i] != 0)
				used_indices.emplace_hint(used_indices.end(), i);
	};

	void fillRedundantIndices()
	{
		redundant_indices.clear();

		if(used_indices.empty())
			return;

		int last_used = *used_indices.rbegin();

		for(int i = 0; i < last_used; ++i)
			if(used_indices.find(i) == used_indices.end())
				redundant_indices.emplace_hint(redundant_indices.end(), i);
	};

	void removeRedundantIndecies(pos & p)
	{
		int deleted_terms = 0;

    size_t initial_size = p.size();

		for(int redundant_index : redundant_indices) {

      if(redundant_index >= initial_size)
        return;

      auto pos_vec_iterator = p.pos_vec.begin();

			std::advance(pos_vec_iterator, redundant_index - deleted_terms);
			p.pos_vec.erase(pos_vec_iterator);

			++deleted_terms;
		}
	};

public:
	template <
    class InputIterator,
    typename = EnableIf<
      std::is_same<
        typename std::iterator_traits<InputIterator>::value_type,
        pos
    > >
  >
	void execute(InputIterator begin, InputIterator end)
	{
		used_indices.clear();

		for(InputIterator it = begin; it != end; ++it)
			fillUsedIndices(*it);

		fillRedundantIndices();

		std::size_t used_len = used_indices.size();

		for(; begin != end; ++begin) {
			removeRedundantIndecies(*begin);
			begin->pos_vec.resize(used_len,0);
		}
	};
};

class ForAllPositions
{
private:
  Command::RelPosCommand * command;

	void SubtractPos(container::iterator begin, container::iterator end, pos p)
	{
		for(; begin != end; ++begin)
			*begin -= p;
	};

public:
	void ForEach(container::iterator begin, container::iterator end)
	{
		for(auto it = begin; it != end; ++it) {

			SubtractPos(begin, end, *it);
			command->Execute(it);
		}
	};

	ForAllPositions(Command::RelPosCommand * command) : command(command) {};
	~ForAllPositions() {};
};

template <std::size_t dim>
class RelPosSummer
{
private:
  Command::AbsPosCommand<dim> * command;
	int indices;
	container::iterator l_begin, l_end;

private:

	void UpdatePositonVectors(std::vector< std::array<int,dim> > & current_state,
                            int index,
                            int direction,
                            int prefactor)
	{
		container::iterator pos_list_it = l_begin;

		for(auto state_iterator = current_state.begin();
		 	  state_iterator != current_state.end();
			  ++state_iterator, ++pos_list_it )
    {
			(*state_iterator)[direction] += (*pos_list_it)[index]*prefactor;
		}
	};

	void AddPositionVectorIndices(std::vector< std::array<int,dim> > & current_state,
                                int index,
                                int direction)
	{
		UpdatePositonVectors(current_state, index, direction, 1);
	};

	void SubtractPositionVectorIndices(std::vector< std::array<int,dim> > & current_state,
                                     int index,
                                     int direction)
	{
		UpdatePositonVectors(current_state, index, direction, -1);
	};

	void SumIndexRecursive(std::vector< std::array<int,dim> > current_state, int index)
	{
		for(int dir = 0; dir < dim; ++dir){

			AddPositionVectorIndices(current_state, index, dir);

			if(index < indices - 1)
				SumIndexRecursive(current_state, index + 1);
			else
				command->Execute(current_state);

			SubtractPositionVectorIndices(current_state, index, dir);
		}
	};

public:

	void ForAll(container::iterator begin, container::iterator end)
	{
		indices = begin->size();
		std::size_t number_of_terms = std::distance(begin, end);

		l_begin = begin; l_end = end;

		std::array<int, dim> zero_array;
		for(int & elem : zero_array)
			elem = 0;

		std::vector< std::array<int,dim> > start_pos(number_of_terms, zero_array);

		SumIndexRecursive(start_pos, 0);
	};

	RelPosSummer(Command::AbsPosCommand<dim> * command)
    : command(command),
      indices(0)
  {};
};

} //Namespace Manipulator
} //Namespace Position

#endif /* POSITION_MANIP_HPP */
