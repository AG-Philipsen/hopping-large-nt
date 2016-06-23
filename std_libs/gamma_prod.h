//Created: 11-09-2013
//Modified: Thu 12 Dec 2013 16:32:49 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef GAMMAPROD_H
#define GAMMAPROD_H

#include<vector>
#include<list>
#include<cstdint>
#include<initializer_list>
#include"error.h"
#include"math_structs.h"
#include"std_funcs.h"

//// A simple class for storing multiplicative states of gamma-matrices such as gigjgigk and so on. It also containts a prefix which is the multiplicity of the configuration.
//// The operators *, *= and - has been overloaded so that gi*gi = 1. The equality operator is overloaded with tracing in mind, it therefore checks all permutations when comparing.
//// The trWeak()-function applies the trace operator in a way that it periodically cancels equal gamma-matrices: gigjgi -> gj. The configuration is stored in a list as it can both
//// pop front and back, which is convenient when multiplying two configurations, and the class contains the convenient splice-function.
//// A couple of functions to manipulate the prefix is also implemented.

class Gamma{
private:
	std::list<std::uint16_t> gammas;
	int prefix;

	void traceRec(DeltaPair**,short*,int,int);
	void traceRec(KD_Expr&,KD_Term&);


public:
	Gamma() : gammas(std::list<std::uint16_t>()), prefix(1) {};
	Gamma(std::uint16_t _index) : gammas(1,_index), prefix(1) {};
	Gamma(std::uint16_t _index, int _prefix) : gammas(1,_index), prefix(_prefix) {};
	Gamma(std::initializer_list<std::uint16_t>);
	Gamma(const Gamma&);
	Gamma(Gamma &&left) : gammas(std::move(left.gammas)), prefix(std::move(left.prefix)) {};

	Gamma operator*(const Gamma&) const;
	Gamma& operator*=(const Gamma&);
	Gamma operator-() const;

	bool operator==(const Gamma&) const;

	void trWeak();
	void addPref(int addition) {prefix += addition;};
	void setPref(int newPref) {prefix = newPref;};
	int getPref() const {return prefix;};
	int size() const {return gammas.size();};

	void trace(DeltaPair**,short*);
	void trace(KD_Expr&);

	void print() const;


};

class GammaError : public Error {
public:
	GammaError(const char *inerr) : Error(inerr,"gamma") {};
};

#endif
