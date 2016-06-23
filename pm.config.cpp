//Created: 04-09-2013
//Modified: Mon 18 Aug 2014 13:53:02 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
//Description: Implementation of the member functions of the class PMConfig

#include"pm.config.h"

namespace hop{

/* Implementation of the [], ==  and = operators */

PMConfig& PMConfig::operator=(const PMConfig& rhs){

  cleanup();

  cfgArray = rhs.cfgArray;
  paths = rhs.paths;
  trace_points = rhs.trace_points;
  prefactor = rhs.prefactor;

  return *this;
}

/// The == operator is implemented in such a way that it rotates through all possible configurations and checks each one as 
/// the configurations are rotation invariant. The operator also makes sure both lhs and rhs are unchanged by the operator,
/// even though it doesn't really make any difference. The class' rotate function is utilised to rotate the configurations.

bool operator==(const PMConfig& lhs, const PMConfig& rhs){

  std::vector<Config> initOrder(lhs.cfgArray);

  for(int n=0; n<lhs.cfgArray.size(); n++){

    if(initOrder == rhs.cfgArray){
      return true;
    }

    std::rotate(initOrder.rbegin(), initOrder.rbegin() + 1, initOrder.rend());
  }
  
  return false;

}

/// The multiplicative incrementation meant to be the way to build up multi trace configurations

void PMConfig::operator*=(const PMConfig &rhs){

  int from = trace_points.empty() ? 0 : trace_points.back();

  int rlen = rhs.cfgArray.size();
  int llen = cfgArray.size();

  if(rlen > (llen - from)){
    throw PMConfigError("While using the operator *= PMConfig:\n"
            "Overflow! The new combined order exceeds this->len.");
  }

  trace_points.push_back(from + rlen);
  prefactor *= rhs.prefactor;

  std::copy(rhs.cfgArray.begin(), rhs.cfgArray.end(), cfgArray.begin() + from);
}

/// A quick (and unecessary) function that rotates the configurations so that its first element is a P and its last is an M. 
/// Basically to have the configurations on the form i wish them to be.

void PMConfig::ordering(){

  while(cfgArray.front() != Cfg_P or cfgArray.back() == Cfg_P){
    std::rotate(cfgArray.rbegin(), cfgArray.rbegin() + 1, cfgArray.rend());
  }
}

/// Finds the upper trace bounds for a given position. E.g. pmpm.pm, given an argument 0-3, returns 4, else 6

int PMConfig::upper_trace_bound(int i) const
{
  return *(std::upper_bound(trace_points.begin(), trace_points.end(), i));
}

/// The lower trace bound need an additional check

int PMConfig::lower_trace_bound(int i) const 
{

  auto it = std::upper_bound(trace_points.begin(), trace_points.end(), i);

  if(it == trace_points.begin()){
    return 0;
  }else{
    --it;
    return *it;
  }
}

/// Quick function to use instead of the destructor. Deletes all elements.

void PMConfig::cleanup(){

  cfgArray.clear();
  paths.clear();
  trace_points.clear();
  prefactor = 1;
}

}; //Namespace hop
