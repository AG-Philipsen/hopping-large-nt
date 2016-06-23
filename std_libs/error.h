//Created: 01-01-2013
//Modified: Thu 19 Dec 2013 12:12:42 CET
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef ERROR_H
#define ERROR_H

#include<iostream>
#include<string>


class Error
{
private:
	std::string err;
	std::string type;

public:
	Error() {};
	Error(const std::string &errmsg) : err(errmsg) {};
	Error(const std::string &errmsg, const std::string &errtype) : err(errmsg), type(errtype) {};
	Error(const char *errmsg) : err(errmsg), type("generic") {};
	Error(const char *errmsg, const char *errtype) : err(errmsg), type(errtype) {};

	const char* c_msg() const {return err.c_str();};
	const char* c_type() const {return type.c_str();};
	const std::string& s_msg() const {return err;};
	const std::string& s_type() const {return type;};

	friend std::ostream& operator<<(std::ostream&, const Error&);

	virtual void printSpecial() {};
};

inline std::ostream& operator<<(std::ostream &os, const Error &err){
	os << "Error message of type _" << err.type << "_ caught. Message:" << std::endl << err.err;
	return os;
}

#endif
