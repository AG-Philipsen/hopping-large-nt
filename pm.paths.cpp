//Created: 09-09-2013
//Modified: Tue 19 Aug 2014 11:32:57 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
//Description: Implementation of the member functions of the class PMPath

#include"pm.paths.h"

namespace hop{

/* -- Print modifier initialisation -- */

/* ============================================================================================================================
 * Small anonymous helper functions to be used by the following member functions
 * ============================================================================================================================ */

namespace{

template<class InputIt>
bool more_than_N_occurances(InputIt first, InputIt last, int N){

	std::map<typename std::iterator_traits<InputIt>::value_type,int> ind_map;

	for(; first != last; ++first){

		auto it = ind_map.find(*first);

		if(it == ind_map.end()){
			ind_map[*first] = 1;
		}else{
			it->second++;

			if(it->second > N){
				return true;
			}
		}
	}

	return false;
}

}; //Anonymous namespace


/// The assignment operator acts even if the left hand side has a different size from the right hand side, in which case the elements destructor is explicitly called
/// and it is reconstructed with a new length.

PMPath& PMPath::operator=(const PMPath &rhs){

	path = rhs.path;
	deltas = rhs.deltas;
	w = rhs.w;

	return *this;
}

/// Function finds the link-point corresponding to the index a. It returns a negative index if the link is behind, and a positive index if it is ahead
/// other than that I'd say the function is rather self-explanatory

int PMPath::findLink(int a) const {

	int mlnk = -path.at(a);

	for(int i=0; i<a; i++){
		if(path[i] == mlnk){
			return -i;
		}
	}

	for(int i=(a+1); i<path.size(); i++){
		if(path[i] == mlnk){
			return i;
		}
	}

	throw PMPathError("Unable to find matching link in function PMPath::findLink(int)");

	return 0;
}

void PMPath::remove_special_spatial_paths(const std::vector<int> &trace_points){

	auto it = s_paths.begin();

	while(it != s_paths.end()){

		int from = 0;

		bool delete_it = false;

		for(int to : trace_points){
			if(more_than_N_occurances(it->begin() + from, it->begin() + to, 2)){
				delete_it = true;
				break;
			}
			from = to;
		}

		if(delete_it){
			cout << "Erased: {";
			for(auto x : *it){
				cout << x << ",";
			}
			cout << "}" << endl;
			it = s_paths.erase(it);
		}else{
			it++;
		}
	}
}

/// Function which finds out the position of the first element of the Wilson-list, and translates all other elements 
/// so that this element is located at x. This function is meant to be run after the Wilson list has been sorted using
/// list::sort()

/*
 *void PMPath::translateCoordinates(){
 *
 *        for(std::vector<WilsonString>::iterator wIt = w.begin(); wIt != w.end(); wIt++){
 *
 *                std::list<int> tmpPos(wIt->wilsons.front().pos);
 *                
 *                //Need to reverse the sign of the position of the first element.
 *                for(std::list<int>::iterator tempIt = tmpPos.begin(); tempIt != tmpPos.end(); tempIt++){
 *                        *tempIt = -(*tempIt);
 *                }
 *
 *                //Then translate all Wilson-elements by this position
 *                for(std::list<Wilson>::iterator it = wIt->wilsons.begin(); it != wIt->wilsons.end(); it++){
 *                        it->translate(tmpPos);
 *                }
 *        }
 *
 *
 *
 *}
 */

}; //Namespace hop
