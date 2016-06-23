//Created: 25-06-2014
//Modified: Tue 14 Oct 2014 13:40:35 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_IO_HPP
#define POSITION_IO_HPP

#include<string>
#include<iostream>
#include<fstream>
#include<streambuf>
#include<algorithm>
#include<boost/lexical_cast.hpp>

#include"position_class.hpp"
#include"../std_funcs.h"
#include"../std_errors.hpp"

namespace Position {

class Printer
{
public:
  virtual void print(const pos &) {};
  virtual ~Printer() {};
};

class Parser
{
public: 
	virtual pos parseString(std::string pos_str) = 0;

	virtual pos parseStream(std::istream & is)
	{
		return parseString( Utility::streamToString(is) );
	};

	virtual pos parseFile(std::string filename)
	{
		std::ifstream is;
		is.open(filename);

		if(!is)
			throw ReadFileException(filename);

		pos result = parseStream(is);
		is.close();

		return result;
	};
};

} //Namespace Position

#endif /* POSITION_IO_HPP */
