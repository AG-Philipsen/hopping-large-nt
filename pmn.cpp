//Created: 18-09-2013
//Modified: Tue 19 Aug 2014 11:30:00 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"pmn.h"

#include"std_libs/subset_sum.hpp"

namespace hop{

PMN::PMN(int _order) : order(_order), configurations(_order/2) {

	if(_order < 2 or _order%2 != 0){
		char errorMsg[256];
		sprintf(errorMsg,"Cannot initialise a class at order %d, which should be fairly obvious",_order);
		throw PMNError(errorMsg);
	}


}

/// Just to make sure the WilsonString we are pointing to isn't deleted when the WilsonStringPtr-struct is unloaded from memory

WilsonStringPtr::~WilsonStringPtr(){

	ptr = nullptr;

}

/// Go through and fill up the list "configurations". The list is a list of vectors of PMPair. The vectors contains all unique
/// orderings of p's and m's, as well as a combinatorial factor as e.q. pmpm and mpmp is the same configuration due to the trace.
/// So the list is over orderings. If allOrders = true, and say order = 6, then the list has three elements, configurations to
/// order k^2, k^4 and k^6. If allOrders = false (which it probably never will be for any other purpose than debugging), then 
/// the list will always be one element long, and that is the specified order. The fillConfigs-function basically fills up the list
/// and the vector with all unique configurations, and counts the configs which are equal.

void PMN::fillConfigs(){

	for(int i=0; i < (order/2); i++){

		int kappa = (i+1)*2;
		
		//First initialise all configurations, and add them to a vector. As there must be an equal number of p's and m's
		//there are binominal(n,n/2) of these.
		int validConfigs = Utility::BinomialCoefficients<int>(kappa, kappa/2);
		std::vector<PMConfig> allConfigs(validConfigs,PMConfig(kappa));

		//The configurations are filled using a recursive function fillAllConfigs, which is explained in more detail later.
		//After this the vector allConfigs should be filled. At e.q. k^4 it is filled with ppmm, pmpm, pmmp, mpmp, mmpp, mppm
		fillAllConfigs(0,(kappa/2+1),kappa/2,allConfigs,0);

		//Do a superficial ordering of all elements so that they all start with a p and end with an m
		for(auto &conf : allConfigs){
			conf.ordering();
		}

		//Add an emply std::vector<PMPair> to the back of the configurations-list
		//configurations.emplace_back;

		for(int j=0; j<validConfigs; j++){

			bool incr = false;
			//Check through the vector of PMPairs and see if the configuration is already present or not
			for(auto &conf : configurations[i]){

				//If it is present, add to the count and move on. The overloaded equality operator
				//also checks all permutations
				if(allConfigs[j] == conf){
					conf++;
					incr=true;
					break;
				}
			}

			//If it wasn't found, add it
			if(!incr){
				configurations[i].push_back(allConfigs[j]);
				configurations[i].back().finalise();
			}
		}

		//Then add its exponential combinatoric factor: -2/order, the 2 from the gamma-trace
		for(auto &conf : configurations[i]){
			conf *= (PM::pref_type)-2;
			conf /= kappa;
		}
	}

	//At order kappa^2 there are no multi-trace configurations
	if(order == 2){
		return;
	}

	int first_multi_trace = configurations.back().size(); //The number of single trace configurations

	//Here we use the subset_sum function defined in std_libs/std_funcs.h. It takes a list of positive integers to 
	//sum and a target, then it finds all possible ways to sum the numbers in the subset so that the result is the target.
	//E.g. {2,4} to 6 gives 2 lists: {2,2,2},{2,4}
	std::vector<int> subset(order/2 - 1);

	for(int i=0; i < (order/2-1); i++){
		subset[i] = (i+1)*2;
	}

	SubsetSum<> sumCreator(order, subset);
	std::list< std::vector<int> > combinations = sumCreator.calculate();

	//A comb is now a list of which PMConfig's we want to construct the multi-trace contrib
	for(auto &comb : combinations){
		configurations.back().emplace_back(order);
		
		std::list< std::vector<int> > send_set;    //This is a parameter sent between the recursive calls to calculate 
		fill_multi_config(comb,0,0,send_set); 		//the combinatoric factor in the end
	}

	//An iterator which starts at the first multi trace
	multi_trace_begin = configurations.back().begin();
	std::advance(multi_trace_begin, first_multi_trace);

	multi_trace_begin_const = multi_trace_begin;
}

/// This is a private, recursive member function which takes in the vector of configurations "conf" and 
/// calculates all possible orderings of p and m. 'left' is how many p's are left to place (as
/// the PMConfig is initialised with all members being m), and the counter is the position of
/// the vector we're at after having added all the new members

int PMN::fillAllConfigs(int from, int to, int left, std::vector<PMConfig> &conf, int counter){

	//First save the initial configuration to a temporary vector, as we will need it
	PMConfig initConf(conf[counter]);

	//If there is only one p left to place, place one at every possible location and return
	if(left == 1){
		for(int i=from; i<to; i++){

			if(i>from){
				conf[counter] = initConf;
			}
			
			conf[counter][i] = Cfg_P;
			counter++;
		}
	}else{
		for(int i=from; i<to; i++){

			if(i>from){
				conf[counter] = initConf;
			}

			conf[counter][i] = Cfg_P;
			
			//If there are more to place, place one and call itself. The to
			//argument is such that if you have 2 left to place, you cannot go
			//to the end of the array, only to the second last element (as there
			//must be room for the last p)
			counter = fillAllConfigs(i+1,to+1,left-1,conf,counter);
		}
	}

	return counter;

}

/// A private recursive function which calculated the multi-trace configurations. To do this, it takes a vector containing which
/// lower level pmConfigs to use. E.g. {2,2,2} results in a pm.pm.pm. It internally sums over all PMConfigs of a given order, and
/// passes the unfinished multi-trace PMConfig to the next level of recursion. In between calls it saves which configurations has
/// been saved to the elements-list, which is also passed on, then used by the last recursive call to calculate the combinatorial
/// factor

void PMN::fill_multi_config(const std::vector<int> &v, int v_ind, int from, std::list< std::vector<int> > elements){

	PMConfig initConf(configurations.back().back());

	bool used = false; //Whether or not we have used the first element
	int c_ind = v[v_ind]/2 - 1; //This is where in the configurations vector we find the order of v[v_ind]

	//Loop through all configurations of the given order. E.g. if v_ind is 4 (kappa^4), then c_ind is 1 and 
	//the for-loop loops over pmpm and ppmm
	for(int i=from; i < configurations[c_ind].size(); i++){

		auto send_set = elements;

		//If we choose the exact same element (both in terms of v_ind and i) we increment how many times
		//we have used that combination to later be able to calculate the combinatorial factor.
		if(send_set.empty()){
			send_set.emplace_back(std::vector<int>({v[v_ind],i,1}));
		}else if( (send_set.back()[0] == v[v_ind]) and (send_set.back()[1] == i) ){
			send_set.back()[2]++;
		}else{
			send_set.emplace_back(std::vector<int>({v[v_ind],i,1}));
		}

		//If we have used the first element, add another one
		if(used){
			configurations.back().emplace_back(initConf);
		}


		//The multiplication operator is overloaded so that it fills in the back if the right hand side
		//is shorter than the right hand side. It also updates the trace_points list
		configurations.back().back() *= configurations[c_ind][i];

		//If there are more elements, recurse
		if( (v_ind + 1) != v.size()){
			fill_multi_config(v,v_ind+1,i,send_set);
		}else{
			//Else, calculate the multiplicative factor from re-ordering. Which is #ofElements!/(product of all
			//individually equal elements!). The numerator is cancelled by the factorial from the series expansion of
			//the exponential.
			PM::pref_type pref_denom = 1;
			for(auto &c_list : send_set){
				pref_denom *= Utility::Factorial<PM::pref_type>(c_list[2]);
			}

			configurations.back().back() /= pref_denom;
		}

		used = true;
	}
}

/// Simple collective function which itterates through all the PM-Configurations and fills in all
/// possible spatial and temporal paths they can take. At the moment, it also removes duplicates
/// and does the gauge integral.

void PMN::fillPaths(){

	if(configurations.empty() or configurations.back().empty()){
		throw PMNError("In function PMN::fillPaths():\n"
				"Cannot fill spate-time orderings of the configurations until they have have been filled.\n"
				"Use PMN::fillConfigs() first.");
	}

	for(auto &conf : configurations.back()){
		conf.populatePaths();
		conf.gaugeIntegrate();
	}

}

/// This function uses the fact that the WilsonString < WilsonString operator gives a strict enough ordering 
/// to check for equal terms, and then delete them (adding together their multiplicative factor). Haven't added a
/// check for whether their multaplicative factors combine to 0 yet. Maybe I will do that if I see that it happens once

/*
 *void PMN::contractWilsons(){
 *
 *        //This iterator is always one step ahead of the main iterator of the for-loop
 *        std::list<WilsonStringPtr>::iterator itp1 = allTerms.begin();
 *
 *        for(std::list<WilsonStringPtr>::iterator it = allTerms.begin(); it != allTerms.end(); it++){
 *
 *                itp1++;
 *
 *                //Using the fact that the list is sorted, we can say that if an element is actually not
 *                //less than the next one, they are in fact equal. E.g. say we have a list {1,1,2,3}, if 
 *                //we then compare 1 < 1 we get false, and hence they must be equal, as the list is sorted.
 *                while( (itp1 != allTerms.end()) and !( *(it->ptr) < *(itp1->ptr) ) ){
 *
 *                        it->prefactor += itp1->prefactor;
 *                        itp1 = allTerms.erase(itp1);
 *                }
 *        }
 *
 *}
 */

}; //Namespace hop
