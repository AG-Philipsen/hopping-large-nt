//Created: 26-11-2013
//Modified: Thu 28 Nov 2013 15:38:30 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef PRINT_LIB_H
#define PRINT_LIB_H

#include<fstream>
#include<iterator>

#include<vector>
#include<list>

template <class Type>
inline std::ostream& operator<<(std::ostream &os, const std::vector<Type> &v){

	for(typename std::vector<Type>::const_iterator it = v.begin(); it != v.end(); it++){
		os << *it << ",";
	}

	return os;
}

template<class Type>
inline std::ostream& operator<<(std::ostream &os, const std::list<Type> &l){

	for(typename std::list<Type>::const_iterator it = l.begin(); it != l.end(); it++){
		os << *it << ",";
	}

	return os;
}

namespace print_lib{

	template <class Type>
	inline void print_vector(std::ostream &os, const std::vector<Type> &v, const char *str){
		std::copy(v.begin(), v.end(), std::ostream_iterator<Type>(os,str));
	}

	template <class Type>
	inline void print_list(std::ostream &os, const std::list<Type> &l, const char *str){
		std::copy(l.begin(), l.end(), std::ostream_iterator<Type>(os,str));
	}

};


#endif
