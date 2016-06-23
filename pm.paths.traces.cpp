//Created: 11-03-2014
//Modified: Mon 18 Aug 2014 11:52:00 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"pm.paths.h"

//// Implementation of PMPath member functions related to the multi-trace paths. Specifically trace displacements and spatial
//// paths

namespace hop{

/// Fills the list of trace displacements and fixes trace-displacement problems. If the trace passes the initial
/// valid_multi_tr_path-check, the configuration is valid, but the trace-displacement might not all match. 
///
/// E.g. a trace might be displaced by both "0" and "i-j", which means that if i=j, the config is valid.

void PMPath::fill_trace_displacements(const std::vector<int> &trace_points){

  Position::pos zero_pos(size()/2);

	auto it = s_paths.begin();

	while(it != s_paths.end()){

		//If the calculation fails, the problematic position (which should be zero), is stored in prob_pos
    Position::pos prob_pos(zero_pos);  
		std::vector<Position::pos> trace_displacements(trace_points.size(),zero_pos);

		//If the calculation fails, we need to impose a delta on prob_pos, which will change the spatial paths,
		//meaning that we need to re-calculate it. The problematic trace is deleted, and correct ones are appended to the
		//end. The trace_disp is only pushed back if the path is OK, meaning that the two lists should be in sync.
		if(!calc_trace_displacements(trace_displacements, zero_pos, prob_pos, trace_points, *it)){
			it = fix_spatial_path(prob_pos,it);
		}else{
			trace_disp.push_back(std::move(trace_displacements));
			++it;
		}
	}

	//Removes paths which are exactly the same after the spatial deltas, also remove the corresponding trace displacements
	co_remove_duplicates(s_paths, trace_disp);  
}

/// Takes an iterator to a problematic spatial spatial configuration, and the position vector which has to be zero for the config
/// to be valid. It then removes the problematic trace (returning an iterator to the next element), and adds the fixed
/// configurations to the end of the s_paths list.

std::list< std::vector<int> >::iterator PMPath::fix_spatial_path(Position::pos &pos, std::list< std::vector<int> >::iterator it){

	std::list< std::vector<int> > new_spatials(1,*it);
	spatial_contractions(pos, new_spatials);

	s_paths.splice(s_paths.end(), std::move(new_spatials));
	std::list< std::vector<int> >::iterator return_val = s_paths.erase(it);

	return return_val;
}

/// Runs a simplified version of calc_trace_displacement, using the PM-config instead of the spatial path to check whether a 
/// given temporal contraction is even possible. If the config survives this test, there exists a way to arrange the spatial
/// path in such a way that it all works out, hence why we do fix_spatial_path in fill_trace_displacements, instead of 
/// deleting the configuration.

bool PMPath::valid_multi_tr_path(const std::vector<Config> &cfgArray, const std::vector<int> &trace_points){

	std::vector<int> rel_pos(trace_points.size(),0);
	int dummy_var;

	return calc_trace_displacements(rel_pos, 0, dummy_var, trace_points, cfgArray);
}

/// This function calculates the displacement of a trace with respect to all the others, and returns a bool corresponding to
/// whether there is a well defined way of doing so. If a trace ends up with two conflicting displacements for a single trace,
/// the false is returned. The function also returns false if not all traces are displaced with respect to zero, as that means
/// that two or more sets of traces are disconnected, and the contribution is already included in the exponensiation.

template <class PosType, class PathType>
bool PMPath::calc_trace_displacements(std::vector<PosType> &rel_pos, const PosType &init_elem, PosType &prob_pos, const std::vector<int> &trace_points, 
		const std::vector<PathType> &sp){

	std::vector<int> disp_wrt(trace_points.size(),0); //List of which traces we are displaces with respect to
	std::vector<char> disp_set(trace_points.size(),false); //Whether the trace displacement has been set

	int from = 0; //trace lower limit
	auto to = trace_points.begin(); //trace upper limit
	int i_tr_ind = 0; //Which trace we are currently in

	for(int i=0; i<path.size(); i++){

		//Update trace limits
		if(i == *to){
			from = *to;
			++to;
			++i_tr_ind;
		}

		//First look for the corresponding link (temporal contractions)
		int lnk = 0;
		for(int j=i+1; j<path.size(); j++){
			if(path[j] == -path[i]){
				lnk = j;
				break;
			}
		}
		
		//If the link is either below or is within the same trace, no displacement need to be set
		if( (lnk == 0) or (lnk < *to) ) continue;

		int lnk_tr_ind = trace_index(lnk, trace_points);

		//The spatial distance between the two elements of the link (excluding the trace displacement)
		PosType current_rel_pos(init_elem);

		int border = (path[i] < 0) ? i+1 : i;

		for(int j=from; j<border; j++){
			current_rel_pos += sp[j];
		}

		border = (path[lnk] < 0) ? lnk+1 : lnk;

		for(int j=trace_points[lnk_tr_ind-1]; j<border; j++){
			current_rel_pos -= sp[j];
		}

		//Which trace index lies at the bottom of the two traces
		int i_root   = disp_set[i_tr_ind]   ? disp_wrt[i_tr_ind]   : i_tr_ind;
		int lnk_root = disp_set[lnk_tr_ind] ? disp_wrt[lnk_tr_ind] : lnk_tr_ind;

		//If they are displaced wrt the same trace, they can be compared
		if(lnk_root == i_root){
			current_rel_pos += rel_pos[i_tr_ind];
			current_rel_pos -= rel_pos[lnk_tr_ind];

			//If the displacement doesn't match the previous one, return false
			if(!pos_is_zero(current_rel_pos)){
				prob_pos = current_rel_pos;
				return false;
			}
		}else{

			//If they are displaced with respect to different traces, one of them needs to be moved.
			//We move the one which has the highest disp_wrt-index, so that in the end, all traces
			//should be displaced wrt 0.
			int move_ind, targ_ind;

			//The total displacement is the relative position plus the two individual
			//displacement, taken in a direction depending on which trace we are moving
			PosType tot_disp(init_elem);

			if(i_root < lnk_root){
				move_ind = lnk_root;
				targ_ind = i_root;
				tot_disp += rel_pos[i_tr_ind];
				tot_disp += current_rel_pos;
				tot_disp -= rel_pos[lnk_tr_ind];
			}else{
				move_ind = i_root;
				targ_ind = lnk_root;
				tot_disp -= rel_pos[i_tr_ind];
				tot_disp -= current_rel_pos;
				tot_disp += rel_pos[lnk_tr_ind];
			}

			//First move the current trace
			rel_pos[move_ind]  = tot_disp;
			disp_wrt[move_ind] = targ_ind;
			disp_set[move_ind] = true;

			//Then move all traces which were displaced with respect to the one just moved
			for(int j=0; j<trace_points.size(); j++){
				if(disp_wrt[j] == move_ind){
					disp_wrt[j] = targ_ind;
					rel_pos[j] += tot_disp;
				}
			}
		}
	}

	//Go through and check if all traces (except 0) are displaced. If not, the contribution is already
	//included in the exponensiation.
	for(int i=1; i<trace_points.size(); i++){
		if(!disp_set[i]){
			return false;
		}
	}

	return true;
}

}; //Namespace hop
