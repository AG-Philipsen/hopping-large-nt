//Created: 06-03-2014
//Modified: Mon 18 Aug 2014 11:54:59 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PM_UTILITY_H
#define PM_UTILITY_H

#include"std_libs/std_funcs.h"
#include"std_libs/position/position_class.hpp"
#include"std_libs/error.h"

#include<vector>
#include<list>
#include<set>

#include"pm.typedef.h"
#include"pm.paths.h"

namespace hop{

int trace_index(int,const std::vector<int>&);

inline bool pos_is_zero(const Position::pos &pos) {return pos.is_zero();};
inline bool pos_is_zero(const int &pos) {return pos == 0;};

template <class PosType>
PosType calculate_position(int,const PosType&,const std::vector<int>&);

template <class PosType>
PosType calculate_position(int,const std::vector<int>&,const std::vector<int>&,const std::vector<PosType>&);

void replace_in_vector(std::vector<int> &, int replace_elem, int replace_with);

void spatial_contractions(Position::pos&, std::list< std::vector<int> >&);

template<class InputIt>
inline int degrees_of_freedom(InputIt first, InputIt last){

	std::set<int> ind_set(first,last);
	return ind_set.size();
}

template <class Type>
inline void remove_duplicates(typename std::vector<Type> &v){

	if(v.size() < 2){
		return;
	}

	for(int i=0; i<v.size(); i++){
		
		typename std::vector<Type>::iterator it = v.begin() + i + 1;

		while(it != v.end()){
			if(*it == v[i]){
				it = v.erase(it);
			}else{
				++it;
			}
		}
	}
}

template <class Type>
inline void remove_duplicates(typename std::list<Type> &l){

	if(l.size() < 2){
		return;
	}

	for(typename std::list<Type>::iterator it_i = l.begin(); it_i != l.end(); ++it_i){

		typename std::list<Type>::iterator it_j = it_i; ++it_j;

		while(it_j != l.end()){
			if(*it_i == *it_j){
				it_j = l.erase(it_j);
			}else{
				++it_j;
			}
		}
	}
}

template <class CheckType, class CoType>
inline void co_remove_duplicates(typename std::list<CheckType> &l, typename std::list<CoType> &co_l){

	if(l.size() < 2){
		return;
	}

	typename std::list<CoType>::iterator it_co_i = co_l.begin();

	for(typename std::list<CheckType>::iterator it_i = l.begin(); it_i != l.end(); ++it_i){

		typename std::list<CheckType>::iterator it_j = it_i; ++it_j;
		typename std::list<CoType>::iterator it_co_j = it_co_i; ++it_co_j;

		while(it_j != l.end()){
			if(*it_i == *it_j){
				it_j = l.erase(it_j);
				it_co_j = co_l.erase(it_co_j);
			}else{
				++it_j;
				++it_co_j;
			}
		}

		++it_co_i;
	}
}

}; //Namespace hop


#endif /* PM_UTILITY_H */
