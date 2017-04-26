//Created: 27-09-2013
//Modified: Wed 26 Apr 2017 17:49:53 BST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
//Description: Implementation of the functions related to the gauge-integral of the PMPath-class.
// 	       Also implementation of the Wilson-struct member functions

#include"pm.paths.h"
#include"std_libs/counted_set.hpp"
#include"pm.config.h"

namespace hop{

/* ============================================================================================================================
 * Small anonymous helper functions to be used by the following member functions
 * ============================================================================================================================ */

namespace{

/// Takes the indecies and trace points and trace-rotates every trace so that the back index moves to the front.
///
/// E.g. tr(abcd)tr(efgh)tr(ij) -> tr(dabc)tr(hefg)tr(ji)

void trace_rotate_indecies(std::vector<int> &indecies, const std::vector<int> &trace_points){

	int from = 0;
	for(int to : trace_points){
		std::rotate(indecies.begin()+2*from, indecies.begin()+2*to-1, indecies.begin()+2*to);
		from = to;
	}
}

template <class Type>
std::ostream& operator<<(std::ostream &os, const std::vector<Type> &v){

	os << "{";

	if(v.empty()){
		os << "}";
		return os;
	}

	os << v[0];

	for(int i=1; i<v.size(); i++){
		os << "," << v[i];
	}
	os << "}";

	return os;

}
};

/// Function which does the gauge integrals and generates the Wilson-lines accordingly. The gauge integrals are done
/// on an index by index basis using the handy formula int dU U_{ij} U^{\dagger}_{kl} = \delta_{il} \delta{jk}
/// For notation conveniance: the C-matrices are the matrix-representation of the inverse of the temporal translation matrix.
/// The function takes in a vector of trace-points, where the point is at the end of where the trace ends. So a 
/// tr(pmpm)tr(pm) has a trPoints vector equal to {4,6}, and a tr(pmpmpm) has {6}

void PMPath::gaugeIntegrate(const std::vector<int> &trace_points){

	//These are the color matrix indecies. These will be manipulated further down and be
	//converted to matrix multiplications and traces. They start at 1. These are the 
	//indecies of the U-matrices, and thus the ones which will be contracted when
	//we do the gauge-integral.
	std::vector<int> indecies(2*path.size());
	for(int i=0; i<indecies.size(); i++){
		indecies[i] = (i+1);
	}

	//Now go through the index array and contract indecies according to the integral given in
	//the description of this function
	for(int i=0; i<path.size(); i++){
		int lnk = findLink(i);
		if(lnk < 0) continue;

		indecies[2*lnk+1] = indecies[2*i];
		indecies[2*lnk] = indecies[2*i+1];
	}

	//Cyclicly rotate the index matrix according to the trace. Basically move the last index of every trace to the front of the same trace
	trace_rotate_indecies(indecies, trace_points);

	std::vector< std::vector<int> > times = set_wilson_positions(trace_points,indecies);

	#ifdef _DEBUG

	for(const WilsonString &wil : w){
		if(wil.wilsons.size() != times.size()){
			throw PMPathError("In function gaugeIntegrate(trace_points):\n"
					  "The number of Wilsons is not the same as the number of times.");
		}
	}

	#endif // _DEBUG

	 /*
	  * Next we will do the sums over the temporal indecies.
	  * The ordering of these is important. Say sum(t1, t2) can be devided into two parts:
	  * sum(t1 < t2) and sum(t2 < t1). which does in theory give different results.
	  * They affect the second identifier in the notation W(n,m), but as 0 < m < n, it is 
	  * not important if n <= 2 (as both W(1) and W(2) can only have m = 1). We therefore
	  * first find all times which are of interest, and only check all possible 
	  * permutation of these. Say we have a WilsonString on the form 
	  * W(3,t1,t2,t3)W(1,t1)W(2,t1,t2)W(2,t3,t4)W(2,t4,t3), then the permutations of t3
	  * and t4 doesn't affect anything, and can be left out, reducing the number of times
	  * we loop through the Permutations struct significantly.
	  */

	std::set<int> time_permutations;

	for(const std::vector<int> &t : times){
		if(t.size() > 2)
			time_permutations.insert(t.begin(), t.end());
	}

	if(time_permutations.empty()){

		for(WilsonString & w_str : w) {
			int signNum = 0;

			for(Wilson & wil : w_str.wilsons)
				signNum += wil.m;

			w_str.prefactor *= (1 - 2*(signNum%2));

      w_str.coordinateCleanup();
		}

		return;
	}

  Utility::CountedSet< std::vector<int> > numerators;

  std::vector<int> time_order(time_permutations.begin(), time_permutations.end());
  size_t number_of_factors = times.size();

  for(Utility::Permutation::Lexicographic< std::vector<int> > perm(time_order.begin(), time_order.end());
      !perm; ++perm) {

    std::vector<int> these_numerators(number_of_factors,0);

		for(int i=0; i < number_of_factors; i++){

      size_t number_of_branches = times[i].size();

			if(number_of_branches < 3){
        these_numerators[i] = 1;
				continue;
			}

			std::vector<int> time_indexed(number_of_branches);

      for(int j = 0; j < number_of_branches; ++j) {
        auto it = std::find(time_order.begin(), time_order.end(), times[i][j]);

        if(it == time_order.end())
          throw std::runtime_error("Could not find the time index");

        time_indexed[j] = std::distance(time_order.begin(), it);
      }

      for(int j = 0; j < (number_of_branches - 1); ++j)
        if(time_indexed[j+1] < time_indexed[j])
          ++these_numerators[i];

      if(time_indexed.front() < time_indexed.back())
        ++these_numerators[i];
		}

    numerators.insert(these_numerators);
	}

	std::list<WilsonString> spatial_terms(std::move(w));
	w.clear();

	PM::pref_type permutation_number = Utility::Factorial<PM::pref_type>(time_permutations.size());

  for(const auto & numerator_config : numerators) {

		for(const WilsonString &sp_wil : spatial_terms){
			w.push_back(sp_wil);

      size_t number_of_factors = numerator_config.obj().size();

			for(int j = 0; j < number_of_factors; ++j)
        w.back().wilsons[j].m = numerator_config.obj()[j];

			w.back().prefactor  = numerator_config.count();
			w.back().prefactor /= permutation_number;

			int signNum = 0;
			for(const Wilson & wil : w.back().wilsons)
				signNum += wil.m;

			w.back().prefactor *= (1 - 2*(signNum%2));
		}
	}

  for(WilsonString & ws : w)
    ws.coordinateCleanup();

/*
 *
 *        //We then sort all the Wilson-lists, and finds the overall sign associated with the given list.
 *
 *        for(std::vector<WilsonString>::iterator wit = w.begin(); wit != w.end(); wit++){
 *
 *                int signNum = 0;
 *                for(std::list<Wilson>::const_iterator it = wit->wilsons.begin(); it != wit->wilsons.end(); it++){
 *                        signNum += it->m;
 *                }
 *
 *                wit->prefactor *= (1 - 2*(signNum%2));
 *                wit->wilsons.sort([] (const Wilson &l, const Wilson &r) {return r < l;});
 *        }
 *        
 *        
 *        //Lastly, we change the Wilson-coordinates so that the first one is located at x
 *        translateCoordinates();
 */

}

std::vector< std::vector<int> > 
PMPath::set_wilson_positions(
    const std::vector<int> & trace_points, 
    const std::vector<int> & indecies)
{
  Position::pos zero_pos(path.size()/2);

	std::list< std::vector<int> >::iterator sp_it = s_paths.begin();
	std::list< std::vector<Position::pos> >::iterator disp_it = trace_disp.begin();

	//The times vector contains a list of which temporal variables go into which Wilson term. They will be the same
	//for every spatial position, so I should only fill it once
	std::vector< std::vector<int> > times;
	bool times_set = false;

	while(sp_it != s_paths.end()){

		//An array which saves which gauge elements have already been included, so that I don't count them multiple times
		std::vector<char> used(path.size(),false);

		w.emplace_back();
		w.back().prefactor = 1;
    w.back().number_of_traces = parent.numberOfTraces();

		for(int i=0; i<path.size(); i++){

			if(used[i]) 
        continue;

			int indexLeft(indecies[2*i]), indexRight(indecies[2*i+1]);

			if(!times_set){
				times.emplace_back();
				times.back().push_back(abs(path[i]));
			}

			w.back().wilsons.emplace_back();
			w.back().wilsons.back().n = 1;
			w.back().wilsons.back().m = 1;

			if(trace_disp.empty()){
				w.back().wilsons.back().pos = calculate_position(i,zero_pos,*sp_it);
			}else{
				w.back().wilsons.back().pos = calculate_position(i,trace_points,*sp_it,*disp_it);
			}

			while(indexLeft != indexRight){
				for(int j=0; j<path.size(); j++){
					if(!used[j] and (indexRight == indecies[2*j])){
						indexRight = indecies[2*j+1];
						used[j] = true;

						if(!times_set){
							times.back().push_back(abs(path[j]));
						}

						w.back().wilsons.back().n++;

						#ifdef _DEBUG //Check if the gauge elements are at the same spatial positions
            Position::pos this_pos;

						if(trace_disp.empty()){
							this_pos = calculate_position(i,zero_pos,*sp_it);
						}else{
							this_pos = calculate_position(i,trace_points,*sp_it,*disp_it);
						}

						if(this_pos != w.back().wilsons.back().pos){
							throw PMPathError("In function gaugeIntegral:\n"
									  "The position of the different elements in a trace are "
									  "at different positions");
						}
						#endif //_DEBUG
					}
				}
			}
		}

		times_set = true;
		sp_it++; if(!trace_disp.empty()) disp_it++;
	}

	return times;
}

}; //Namespace hop
