//Created: 06-03-2014
//Modified: Mon 18 Aug 2014 12:08:55 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"pm.utility.h"

namespace hop{

int trace_index(int n, const std::vector<int> &trace_points){

	for(int i=0; i<trace_points.size(); i++){
		if(n < trace_points[i]){
			return i;
		}
	}

	throw Error("The index is not within the trace_points","pm_utility");
}

void rectify_spatial_path(Position::pos &pos, PMPath &p, std::list< std::vector<int> >::iterator it){

	std::list< std::vector<int> > new_spatials(1,*it);
	spatial_contractions(pos, new_spatials);

	p.s_paths.erase(it);
	p.s_paths.splice(p.s_paths.end(), std::move(new_spatials));
}

template <class PosType>
PosType calculate_position(int index, const PosType &zero_pos, const std::vector<int> &s_path){

	PosType ret_pos(zero_pos);

	for(int i=0; i<index; i++){
		ret_pos += s_path[i];
	}

	return ret_pos;
}

template <class PosType>
PosType calculate_position(int index, const std::vector<int> &trace_points, const std::vector<int> &s_path, const std::vector<PosType> &tr_pos){

	int tr_ind = trace_index(index,trace_points);
	int from = (tr_ind == 0) ? 0 : trace_points[tr_ind-1];

	PosType ret_pos(tr_pos[tr_ind]);

	for(int i=from; i<index; i++){
		ret_pos += s_path[i];
	}

	return ret_pos;
}

void replace_in_vector(std::vector<int> &v, int replace_elem, int replace_with){
	
	for(int &i : v){
		if(abs(i) == replace_elem){
			i = Utility::sign(i) * replace_with;
		}
	}
}

void spatial_contractions(Position::pos &p, std::list< std::vector<int> > &spatial_paths){

	int start = 0;
	int end = p.size();

	while(start != end and p[start] == 0){
		++start;
	}

	if(start == end){
		return;
	}

	int init_sign = Utility::sign(p[start]);
	std::vector<int> init_spatial(spatial_paths.back());

	bool first_used = false;

	for(int i=start+1; i<end; i++){

		int i_sign = Utility::sign(p[i]);

		if(init_sign == -i_sign){

			if(first_used){
				spatial_paths.push_back(init_spatial);
			}else{
				first_used = true;
			}

			replace_in_vector(spatial_paths.back(), i + 1, start + 1);

			int i_tmp = p[i];
			p[start] += i_tmp; p[i] = 0;

			spatial_contractions(p, spatial_paths);

			p[start] -= i_tmp; p[i] = i_tmp;

		}
	}

	if(!first_used){
		throw Error("The spatial vector isn't zero in the end");
	}
}

}; //Namespace hop

template Position::pos hop::calculate_position(int,const Position::pos &,const std::vector<int> &);
template Position::pos hop::calculate_position(int,const std::vector<int>&,const std::vector<int>&,const std::vector<Position::pos>&);
