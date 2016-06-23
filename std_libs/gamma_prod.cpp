//Created: 11-09-2013
//Modified: Thu 26 Jun 2014 15:47:59 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)
//Description: Implementation of the member functions of the class Gamma initialised in "gammaProd.h"

#include"gamma_prod.h"
#include<boost/math/special_functions/factorials.hpp>

//Comment

Gamma::Gamma(const Gamma &rhs) : gammas(rhs.gammas), prefix(rhs.prefix) {};

Gamma::Gamma(std::initializer_list<std::uint16_t> l) : gammas(l), prefix(1) {};


/// The multiplication operator. It multiplies two configurations, but takes into account the properties of the gamma matrices. Specifically
/// the fact that gi*gi = 1

Gamma Gamma::operator*(const Gamma &right) const {

	//Need to store the matrices in temporary copes as we don't want to change
	//the input-configurations when we pop and splice
	Gamma retG(*this), tmpG(right);
	retG.prefix *= tmpG.prefix;

	//An empty list corresponds to the unity operator, and therefore we return accordingly
	if(retG.gammas.empty()){
		tmpG.prefix *= prefix;
		return tmpG;
	}else if(tmpG.gammas.empty()){
		return retG;
	}

	while(retG.gammas.back() == tmpG.gammas.front()){
		retG.gammas.pop_back();
		tmpG.gammas.pop_front();

		//Again, need to check if any of them empties, as reading back and front of empty lists returns random junk, which can mess
		//the calculations up
		if(retG.gammas.empty()){
			tmpG.prefix *= prefix;
			return tmpG;
		}else if(tmpG.gammas.empty()){
			return retG;
		}

	}

	//At last we splice the lists when there are no more matching gammas
	retG.gammas.splice(retG.gammas.end(), tmpG.gammas);

	return retG;

}

/// Basically the exact same thing as the overloading of the *-operator

Gamma& Gamma::operator*=(const Gamma &rhs){

	prefix *= rhs.prefix;

	Gamma tmp(rhs);
	
	if(gammas.empty()){
		gammas.splice(gammas.end(), tmp.gammas);
		return *this;
	}else if(rhs.gammas.empty()){
		return *this;
	}

	while(gammas.back() == rhs.gammas.front()){
		gammas.pop_back();
		tmp.gammas.pop_front();

		if(gammas.empty()){
			gammas.splice(gammas.end(), tmp.gammas);
			return *this;
		}else if(rhs.gammas.empty()){
			return *this;
		}
	}


	gammas.splice(gammas.end(), tmp.gammas);
	return *this;
}

/// The equality operator checks all permutations as well, but as there are some complications (at least in my head)
/// due to the fact that one has to scan through the lists with iterators, I chose to save the configuration in a 
/// temporary int array when rotating and checking permutations

bool Gamma::operator==(const Gamma &rhs) const{

	if(gammas.size() != rhs.gammas.size()){
		return false;
	}

	if(gammas.empty()){
		return true;
	}

	int length = rhs.gammas.size();
	std::uint16_t tmp[length];

	int i = 0;
	for(std::list<std::uint16_t>::const_iterator it = rhs.gammas.cbegin(); it!=rhs.gammas.cend(); it++){
		tmp[i] = *it;
		i++;
	}

	//The standard way of checking all permutations
	for(int n=0; n<length; n++){
		
		bool same = true;

		std::list<std::uint16_t>::const_iterator it1;
		i=0;
		for(it1=gammas.cbegin(); it1!=gammas.cend(); it1++){
			if(*it1 != tmp[i]){
				same = false;
				break;
			}

			i++;
		}

		if(same){
			return true;
		}

		int tmpStart = tmp[length-1];
		for(i = length-1; i>0; i--){
			tmp[i] = tmp[i-1];
		}
		tmp[0] = tmpStart;

	}

	return false;

}

/// The negation operator only changes the prefix

Gamma Gamma::operator-() const {

	Gamma retGamma(*this);
	retGamma.prefix = -retGamma.prefix;

	return retGamma;


}

/// trWeak also cancels gammas periodically, works the same way with pop'ing as the overloaded *-operator

void Gamma::trWeak(){

	if(gammas.empty()){
		return;
	}

	while(gammas.front() == gammas.back()){
		gammas.pop_front();
		gammas.pop_back();

		if(gammas.empty() or gammas.size() == 1){
			return;
		}
	}

}

/// Simple function to print the gamma product

void Gamma::print() const {

	if(gammas.empty()){
		std::printf("{1}");
	}else{
		std::printf("{");
		for(std::list<std::uint16_t>::const_iterator it = gammas.cbegin(); it!=gammas.cend(); it++){
			std::printf("%c",(char)((std::uint16_t)'i'+ *it - 1));
		}
		std::printf("}");
	}

}

void Gamma::trace(DeltaPair **deltas, short *signs){

	if(gammas.size()%2 != 0){
		throw GammaError("In function trace():\n"
				 "Taking the trace of an odd number of gammas, something I haven't implemented.");
	}

	trWeak();

	traceRec(deltas,signs,0,0);

}

void Gamma::traceRec(DeltaPair **deltas, short *signs, int indI, int indJ){

	if(gammas.size() == 2){
		deltas[indI][indJ] = {gammas.front(),gammas.back()};
		return;
	}

	int copies = static_cast<int>(boost::math::double_factorial<double>(gammas.size() - 3));

	for(int i=1; i<gammas.size(); i++){

		Gamma sendGam(*this);
		std::list<std::uint16_t>::iterator git = sendGam.gammas.begin();
		std::advance(git, i);

		int thisSign = (2*(i%2) - 1);

		for(int n = 0; n<copies; n++){
			deltas[indI + n + (i-1)*copies][indJ] = {gammas.front(),*git};
			signs[indI + n + (i-1)*copies] *= thisSign;
		}

		sendGam.gammas.pop_front();
		sendGam.gammas.erase(git);

		sendGam.traceRec(deltas, signs, indI+(i-1)*copies, indJ+1);

	}
}

void Gamma::trace(KD_Expr &deltas){
	if(gammas.size()%2 != 0){
		throw GammaError("In function trace():\n"
				 "Taking the trace of an odd number of gammas, something I haven't implemented.");
	}

	trWeak();
	KD_Term dt(prefix);

	if(gammas.empty()){
		deltas += dt;
		return;
	}

	traceRec(deltas,dt);
}

void Gamma::traceRec(KD_Expr &de, KD_Term &dt){

	if(gammas.size() == 2){

		dt.push_back(gammas.front(), gammas.back());
		de += std::move(dt);
		return;
	}

	for(int i=1; i<gammas.size(); i++){
		KD_Term pass_dt(dt);

		Gamma sendGam(*this);
		std::list<std::uint16_t>::iterator git = sendGam.gammas.begin();
		std::advance(git, i);

		int thisSign = (2*(i%2) - 1);

		pass_dt *= thisSign;
		pass_dt.push_back(sendGam.gammas.front(), *git);

		sendGam.gammas.pop_front();
		sendGam.gammas.erase(git);

		sendGam.traceRec(de,pass_dt);
	}
}
