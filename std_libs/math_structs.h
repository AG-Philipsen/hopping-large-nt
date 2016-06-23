//Created: 15-10-2013
//Modified: Tue 25 Feb 2014 17:20:55 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef MATHSTRUCTS_H
#define MATHSTRUCTS_H

#include<cstdio>
#include<iostream>

#include<list>
#include<set>
#include<initializer_list>

#include<boost/rational.hpp>

#include<cstdint>
#include<algorithm>
#include"error.h"

namespace MathStructs{
	typedef std::uint16_t kd_ind;
	typedef long pref_type;
};

struct DeltaPair{
	MathStructs::kd_ind one;
	MathStructs::kd_ind two;
};

struct KD_Factor{

	std::set<MathStructs::kd_ind> deltas;

	KD_Factor() {};
	KD_Factor(MathStructs::kd_ind,MathStructs::kd_ind);
	KD_Factor(std::initializer_list<MathStructs::kd_ind> l) : deltas(l) {};
	KD_Factor(const KD_Factor &d) : deltas(d.deltas) {};
	KD_Factor(KD_Factor &&left) : deltas(std::move(left.deltas)) {};

	KD_Factor& operator=(const KD_Factor &rhs) {deltas = rhs.deltas; return *this;};
	KD_Factor& operator=(KD_Factor &&rhs) {std::swap(deltas, rhs.deltas); return *this;};

	bool operator<(const KD_Factor &r) const {return deltas < r.deltas;};
	bool operator==(const KD_Factor &r) const {return deltas == r.deltas;};
	bool operator!=(const KD_Factor &r) const {return deltas != r.deltas;};
	
	MathStructs::kd_ind operator()(MathStructs::kd_ind) const;
	MathStructs::kd_ind operator()(MathStructs::kd_ind,bool&) const;

	//bool push_back(MathStructs::kd_ind);
	bool push_back(MathStructs::kd_ind,MathStructs::kd_ind);
	bool push_back(const KD_Factor&);

	friend std::ostream& operator<<(std::ostream&,const KD_Factor&);
};

struct KD_Term{

	std::set<KD_Factor> delta_list;
	boost::rational<MathStructs::pref_type> pref;

	static int print_pref;

	KD_Term() : pref(1) {};
	KD_Term(MathStructs::pref_type num) : pref(num) {};
	KD_Term(MathStructs::pref_type num, MathStructs::pref_type den) : pref(num,den) {};
	KD_Term(boost::rational<MathStructs::pref_type> p) : pref(p) {};
	KD_Term(std::initializer_list< std::initializer_list<MathStructs::kd_ind> >);
	KD_Term(const KD_Term &left) : delta_list(left.delta_list), pref(left.pref) {};
	KD_Term(KD_Term &&left) : delta_list(std::move(left.delta_list)), pref(std::move(left.pref)) {};

	KD_Term& operator=(const KD_Term&);
	KD_Term& operator=(KD_Term &&);

	KD_Term& operator*=(MathStructs::pref_type x) {pref *= x; return *this;};
	KD_Term& operator*=(boost::rational<MathStructs::pref_type> x) {pref *= x; return *this;};
	KD_Term& operator*=(const KD_Factor&);
	KD_Term& operator*=(const KD_Term&);
	KD_Term& operator/=(MathStructs::pref_type x) { pref /= x; return *this;};
	KD_Term& operator/=(boost::rational<MathStructs::pref_type> x) { pref /= x; return *this;};
	bool operator<(const KD_Term &r) const {return delta_list < r.delta_list;};
	bool operator==(const KD_Term &r) const {return delta_list == r.delta_list;};
	
	KD_Term operator* (MathStructs::pref_type x) {KD_Term ret_term(*this); ret_term *= x; return ret_term;};
	KD_Term operator* (boost::rational<MathStructs::pref_type> x) {KD_Term ret_term(*this); ret_term *= x; return ret_term;};

	MathStructs::kd_ind operator()(MathStructs::kd_ind) const;

	void push_back(MathStructs::kd_ind x, MathStructs::kd_ind y) { if(x != y) *this *= KD_Factor(x, y); };

	std::set<KD_Factor>::iterator begin() {return delta_list.begin();};
	std::set<KD_Factor>::iterator end() {return delta_list.end();};

	bool empty() const;

	//void print(FILE *out=stdout, char sep = 0) const;
	friend std::ostream& operator<<(std::ostream&, const KD_Term&);

	static std::ios_base& prefactor_on(std::ios_base &io) {io.iword(print_pref) = 1; return io;};
	static std::ios_base& prefactor_off(std::ios_base &io) {io.iword(print_pref) = 0; return io;};

	void cleanup();
};

struct KD_Expr{

	std::set<KD_Term> deltas;

	KD_Expr() {};
	KD_Expr(const KD_Expr &r) : deltas(r.deltas) {};
	KD_Expr(KD_Expr &&r) : deltas(std::move(r.deltas)) {};

	KD_Expr& operator=(const KD_Expr &r) { deltas = r.deltas; return *this; };
	KD_Expr& operator=(KD_Expr &&r) { std::swap(deltas, r.deltas); return *this; };

	KD_Expr  operator* (const KD_Term &rhs) const {KD_Expr ret(*this); ret *= rhs; return ret;};

	KD_Expr& operator+=(const KD_Term &);
	KD_Expr& operator+=(KD_Term &&);
	KD_Expr& operator+=(const KD_Expr &);
	KD_Expr& operator+=(KD_Expr &&);
	KD_Expr& operator*=(MathStructs::pref_type);
	KD_Expr& operator*=(const boost::rational<MathStructs::pref_type> &);
	boost::rational<MathStructs::pref_type> pref;
	KD_Expr& operator*=(const KD_Factor &);
	KD_Expr& operator*=(const KD_Term &);
	KD_Expr& operator*=(const KD_Expr &);

	std::set<KD_Term>::iterator begin() {return deltas.begin();};
	std::set<KD_Term>::iterator end() {return deltas.end();};

	bool empty() const {return deltas.empty();};

	friend std::ostream& operator<<(std::ostream&, const KD_Expr&);

	bool is_zero() const {return deltas.empty();};
};

#endif
