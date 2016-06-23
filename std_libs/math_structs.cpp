//Created: 15-10-2013
//Modified: Tue 25 Feb 2014 16:54:17 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#include"math_structs.h"

/* KD_Factor struct functions */

KD_Factor::KD_Factor(MathStructs::kd_ind x, MathStructs::kd_ind y) {

	if(x == y) return;

	deltas.insert(x);
	deltas.insert(y);
}

MathStructs::kd_ind KD_Factor::operator()(MathStructs::kd_ind x) const {

	if(deltas.find(x) != deltas.end()){
		return *deltas.begin();
	}else{
		return x;
	}
}

MathStructs::kd_ind KD_Factor::operator()(MathStructs::kd_ind x, bool &found) const {

	if(deltas.find(x) != deltas.end()){
		found = true;
		return *deltas.begin();
	}else{
		found = false;
		return x;
	}
}

bool KD_Factor::push_back(MathStructs::kd_ind x, MathStructs::kd_ind y){

	auto itx = deltas.insert(x);
	auto ity = deltas.insert(y);

	if(itx.second and ity.second){
		deltas.erase(itx.first);
		deltas.erase(ity.first);
		return false;
	}

	return true;
}

bool KD_Factor::push_back(const KD_Factor &right){

	bool to_append = false;
	for(auto x : right.deltas){
		if(deltas.find(x) != deltas.end()){
			to_append = true;
			break;
		}
	}

	if(!to_append){
		return false;
	}

	for(auto x : right.deltas){
		deltas.insert(x);
	}

	return true;
}

std::ostream& operator<<(std::ostream &os, const KD_Factor &d){

	//MathStructs::kd_ind lett = (MathStructs::kd_ind)'i' - 1;

	os << "{";
	for(const MathStructs::kd_ind i : d.deltas){
		//os << (char)(lett + i);
		os << i;
	}
	os << "}";

	return os;

}

/* KD_Term struct functions */

int KD_Term::print_pref = std::ios_base::xalloc();

KD_Term::KD_Term(std::initializer_list< std::initializer_list<MathStructs::kd_ind> > l){

	for(auto &ll : l){
		delta_list.insert(KD_Factor(ll));
	}
}

KD_Term& KD_Term::operator=(const KD_Term &l){

	delta_list = l.delta_list;
	pref = l.pref;
	return *this;
}

KD_Term& KD_Term::operator=(KD_Term &&l){

	std::swap(delta_list, l.delta_list);
	std::swap(pref, l.pref);

	return *this;
}

KD_Term& KD_Term::operator*=(const KD_Factor &right) {

	std::list< std::set<KD_Factor>::iterator > matching; 

	for(auto it = delta_list.begin(); it != delta_list.end(); it++){

		for(auto x : right.deltas){
			if(it->deltas.find(x) != it->deltas.end()){
				matching.push_back(it);
				break;
			}
		}
	}

	if(matching.empty()){
		delta_list.insert(right);
		return *this;
	}

	auto itit = matching.begin();

	KD_Factor new_term(right);

	while(itit != matching.end()){
		new_term.deltas.insert((*itit)->deltas.begin(), (*itit)->deltas.end());
		delta_list.erase(*itit);
		++itit;
	}

	delta_list.insert(std::move(new_term));

	return *this;
}

KD_Term& KD_Term::operator*=(const KD_Term &right){

	pref *= right.pref;

	for(const auto &d : right.delta_list){
		*this *= d;
	}

	return (*this);

}

MathStructs::kd_ind KD_Term::operator()(MathStructs::kd_ind x) const {

	for(auto &kd : delta_list){
		bool found;

		MathStructs::kd_ind tmp = kd(x,found);

		if(found){
			return tmp;
		}
	}

	return x;
}

bool KD_Term::empty() const{
	if(delta_list.empty()){
		return true;
	}

	for(const auto &kd : delta_list){
		if(!kd.deltas.empty()){
			return false;
		}
	}

	return true;
}

std::ostream& operator<<(std::ostream &os, const KD_Term &kl){

	if(os.iword(KD_Term::print_pref) == 1){
		os << kl.pref;
	}

	for(const KD_Factor &kd : kl.delta_list){
		os << kd;
	}

	if(kl.delta_list.empty()){
		os << "{}";
	}

	return os;

}

/* KD_Expr struct functions */

KD_Expr& KD_Expr::operator+=(const KD_Term &dt){

	auto find_it = deltas.find(dt);

	if(find_it == deltas.end()){
		deltas.insert(dt);
	}else{
		KD_Term new_term(*find_it);
		deltas.erase(find_it);

		new_term.pref += dt.pref;
		if(new_term.pref != 0){
			deltas.insert(new_term);
		}
	}

	return *this;
}

KD_Expr& KD_Expr::operator+=(KD_Term &&dt){

	auto find_it = deltas.find(dt);

	if(find_it == deltas.end()){
		deltas.insert(std::move(dt));
	}else{
		KD_Term new_term(*find_it);
		deltas.erase(find_it);

		new_term.pref += dt.pref;
		if(new_term.pref != 0){
			deltas.insert(new_term);
		}
	}

	return *this;
}

KD_Expr& KD_Expr::operator+=(const KD_Expr &rhs){

	if(empty()){
		*this = rhs;
		return *this;
	}

	for(const auto &dt : rhs.deltas){
		*this += dt;
	}

	return *this;
}

KD_Expr& KD_Expr::operator+=(KD_Expr &&rhs){

	if(empty()){
		*this = std::move(rhs);
		return *this;
	}

	for(auto it = rhs.deltas.begin(); it != rhs.deltas.end();){
		operator+=(std::move(const_cast<KD_Term &>(*it)));
		it = rhs.deltas.erase(it);
	}
	
	return *this;
}

KD_Expr& KD_Expr::operator*=(MathStructs::pref_type x){

	std::set<KD_Term> new_set;

	for(const auto &d : deltas){
		KD_Term new_term(d);
		new_term.pref *= x;
		new_set.insert(std::move(new_term));
	}

	std::swap(deltas, new_set);

	return *this;
}

KD_Expr& KD_Expr::operator*=(const boost::rational<MathStructs::pref_type> &x){

	std::set<KD_Term> new_set;

	for(const auto &d : deltas){
		KD_Term new_term(d);
		new_term.pref *= x;
		new_set.insert(std::move(new_term));
	}

	std::swap(deltas, new_set);

	return *this;
}

KD_Expr& KD_Expr::operator*=(const KD_Factor &d){

	std::set<KD_Term> all_terms(std::move(deltas));

	for(auto it = all_terms.begin(); it != all_terms.end();){

		KD_Term new_term(std::move(const_cast<KD_Term &>(*it)));
		it = all_terms.erase(it);
		new_term *= d;
		operator+=(std::move(new_term));
	}

	return *this;
}

KD_Expr& KD_Expr::operator*=(const KD_Term &df){

	std::set<KD_Term> all_terms(std::move(deltas));

	for(auto it = all_terms.begin(); it != all_terms.end();){

		KD_Term new_term(std::move(const_cast<KD_Term &>(*it)));
		it = all_terms.erase(it);
		new_term *= df;
		operator+=(std::move(new_term));
	}

	return *this;
}

KD_Expr& KD_Expr::operator*=(const KD_Expr &rhs){

	if(empty()){
		return *this = rhs;
	}

	KD_Expr original_term(std::move(*this));
	deltas.clear();

	for(const KD_Term &d : rhs.deltas){

		KD_Expr multipl_term(original_term);
		multipl_term *= d;

		operator+=(std::move(multipl_term));
	}

	return *this;

}

std::ostream& operator<<(std::ostream &os, const KD_Expr &de){

	for(const KD_Term &dt : de.deltas){
		os << dt << " ";
	}

	if(de.deltas.empty()){
		os << 0;
	}

	return os;
}
