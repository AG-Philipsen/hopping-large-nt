//Created: 19-09-2013
//Modified: Wed 26 Apr 2017 17:55:05 BST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
//Description: Implementation of the functions directly related to the paths

#include"pm.config.h"

namespace hop{

/* ============================================================================================================================
 * Small anonymous helper functions to be used by the following member functions
 * ============================================================================================================================ */

namespace{

bool no_open_links_exist_between(std::vector<char> &used, int from, int to){

  bool open_in_used = (std::find(used.begin() + from + 1, used.begin() + to, false) == (used.begin() + to));
  return open_in_used or (to-from < 3);
}

bool is_null_vector(const std::vector<int> &v){
  return std::all_of(v.begin(), v.end(), [](const int &x){return x == 0;});
}

void fill_used_external(std::vector<char> &used, const PMPath &p, std::vector<int> trace_points){

  int from = 0;
  for(int to : trace_points){
    for(int i=from; i<to; i++){
      int link = abs(p.findLink(i));
      
      if( (link < from) or (link >= to) ){
        used[i] = false;
      }else{
        used[i] = true;
      }
    }
    from = to;
  }
}

}; //Anonymous namespace

/// The initialisation function to populate the vector "paths" with all possible space-time paths. It calls either the function
/// branchPathsCorrect or branchPathsAll, which themselves are recursive functions that will be described in detail below.

void PMConfig::populatePaths(){

  paths.emplace_back(cfgArray.size(), *this);

  std::vector<char> used(cfgArray.size());
  for(int i=0; i<cfgArray.size(); i++){
    used[i] = false;
  }

  temporal_connections(0, cfgArray.size(), used, 1);

  if(!is_single()){
    auto it = paths.begin();


    while(it != paths.end()){
      //if(!valid_multi_tr_path(*it)){
      std::vector<int> displacements(trace_points.size(),0);
      if(!it->valid_multi_tr_path(cfgArray,trace_points)){
        it = paths.erase(it);
        continue;
      }
      it++;
    }
  }

  fill_delta();
}

/// Function which connects a P with an M to form a "link", meaning that they shall happen at the same space-time coordinate.
/// At the moment, this function also disallows spatial loops, as they do not contribute when the gauge integral is done.
//
/// Note on multi trace configurations:

void PMConfig::temporal_connections(int from, int to, std::vector<char> &used, int link_id){ 

  //First look for the first unused linking point
  int start = from;
  while(used[start] and start!=to){
    start++;
  }

  //If there is none, exit
  if(start == to){
    return;
  }

  //The upper limit for the current trace
  int upper = upper_trace_bound(start);

  //Push back the start-position, and make a copy of the state
  paths.back()[start] = cfgArray[start] == Cfg_P ? link_id : -link_id;
  paths.back().s_paths.front()[start] = paths.back()[start];
  PMPath init_path(paths.back());

  Config init_cfg = cfgArray[start];
  int count_p(0), count_m(0);

  //The start-array is passed around in every recursive call, so I only set its
  //value temporarily. It is unset before I return
  used[start] = true;

  bool first_used = false;

  for(int i=(start+1); i<to; i++){

    // This if-statement checks if the link is valid with the following rules:
    //   - A P must link to an M, and visa versa
    //   - The link point cannot be in use by another link
    //   - either:
    //    * There must be an equal number of p's and m's between the link, so backtracking is possible
    //    * One must link outside of the current trace, making e.g. insertions possible
    if( (cfgArray[i]!=init_cfg) and !used[i] and ( (count_p == count_m) or i >= upper ) ){

      //If the last path has already been filled, append a copy of
      //the initial path at the end
      if(first_used){
        paths.emplace_back(init_path);
      }
      paths.back()[i] = -paths.back()[start];
      paths.back().s_paths.front()[i] = paths.back()[i];

      //Then set the current link-statue to used and recurse
      used[i] = true;
      temporal_connections(start+1, to, used, link_id+1);
      used[i] = false;

      first_used = true;
    }

    cfgArray[i] == Cfg_P ? ++count_p : ++count_m;
  }

  // As the multi-trace configurations can start building invalid path configurations, we have to remove it if no link 
  // could be made
  if(!first_used){
    paths.pop_back();
  }

  used[start] = false;
}

/// Iterates over all the paths and calls the single-path crossing function assuming no trace-points

void PMConfig::complete_single_trace(){
  for(auto &p : paths){
    single_path_crossing(0,cfgArray.size(),p);
  }
}

/// The function corresponding to complete_single_trace, but for multi-traces. There are multiple things which has
/// to be done to make sure the spatial parts of the multi-trace is in order:
///   1. Make sure we are taking the spatial trace correctly, meaning that the spatial position of the last element
///      is the same as the spatial position of the first element
///   2. The connected elements must be at the same spatial position, similar to how it must be for the 
///      single trace elements, but the disconnected traces can themselves be spatially displaced, making it
///      more complicated.

void PMConfig::complete_multi_trace(){

  std::vector<char> used(cfgArray.size());

  for(PMPath &p : paths){

    int from = 0;
    for(auto to : trace_points){
      positional_delta(from, to, p); //takes care of point 1
      single_path_crossing(from, to, p); //takes partially care of point 2
      from = to;
    }


    p.fill_trace_displacements(trace_points); //fixes the final issues of point 2

    if(p.s_paths.size() < 2){
      continue;
    }


    //Remove all paths which have a lesser amount of free indecies than the others.
    //They are most likely duplicates of some sort.

    std::vector<int> dof(p.s_paths.size());

    int i=0;
    for(std::vector<int> &s_p : p.s_paths){
      dof[i] = degrees_of_freedom(s_p.begin(), s_p.end());
      i++;
    }

    int highest_dof = *(std::max_element(dof.begin(), dof.end()));

    i = 0;
    auto it = p.s_paths.begin();
    while(it != p.s_paths.end()){
      if(dof[i] < highest_dof){
        it = p.s_paths.erase(it);
      }else{
        ++it;
      }
      i++;
    }
  }
}

/// The idea of the crossing functions is to take a link, say PmpMpm and make sure that P and M is at the 
/// same spatial positions. This is done by making sure that the full accumulated spatial path from
/// the mp in between is 0. The single_path_crossing takes a from and to, which is the beginning and the 
/// end of the current trace. It then makes sure all links are at the same spatial position.

void PMConfig::single_path_crossing(int from, int to, PMPath &path){

  KD_Expr ret_expr;
  std::vector<char> used(cfgArray.size(),false);

  for(int i=from; i<to; i++){

    int lnk_val = path[i];
    auto lnk_it = std::find_if(path.path.begin()+i+1, path.path.begin()+to, [lnk_val](const int &x){return x == -lnk_val;});

    if(lnk_it == (path.path.begin()+to)) continue;

    int lnk_from = i + 1;
    int lnk_to   = std::distance(path.path.begin(), lnk_it);

    positional_delta(lnk_from, lnk_to, path); //Spatial kronecker delta between from and to
  }
}

/// Takes the indecies of two link positions which should be at the same spatial position, and iterates through
/// all spatial paths, making sure that is fulfilled. Technically, from is the position after the first link-position.
/// A challenge is that setting a positional vector to zero can in some cases be done in more than one way. Meaning that
/// we can spawn new spatial configurations every time we try to impose a delta.

void PMConfig::positional_delta(int from, int to, PMPath &path){

  //List to save all new spatial paths which might spawn
  std::list< std::vector<int> > new_s_paths;
  
  for(auto &sp : path.s_paths){

    Position::pos pos(path.size()/2);
    for(int i=from; i<to; i++){
      pos += sp[i];
    }

    //Temporary list to pass to the delta-function, which contains one copy of
    //the current spatial path.
    std::list< std::vector<int> > send_paths(1,sp);
    spatial_contractions(pos,send_paths);

    new_s_paths.splice(new_s_paths.end(), std::move(send_paths));
  }

  remove_duplicates(new_s_paths);
  path.s_paths = std::move(new_s_paths);
}

}; //Namespace hop
