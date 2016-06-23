//Created: 25-06-2014
//Modified: Wed 25 Jun 2014 17:10:52 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef STD_ERRORS_HPP
#define STD_ERRORS_HPP

#include<exception>
#include<stdexcept>
#include<string>
#include<sstream>

class ReadFileException : public std::exception
{
private:
	std::string filename;

public:
	const char* what() const throw()
	{
		std::stringstream ss;
		ss << "Unable to open file: \"" << filename << "\" for reading.";
		return ss.str().c_str();
	};

	ReadFileException(const std::string &fname) : filename(fname) {};
	ReadFileException(const char *fname) : filename(fname) {};
};

class WriteFileException : public std::exception
{
private:
	std::string filename;

public:
	const char* what() const throw()
	{
		std::stringstream ss;
		ss << "Unable to open file: \"" << filename << "\" for writing.";
		return ss.str().c_str();
	};

	WriteFileException(const std::string &fname) : filename(fname) {};
	WriteFileException(const char *fname) : filename(fname) {};
};

class OutOfRangeException : public std::exception
{
public:
	const char * what() const throw()
	{
		return "Out of range of container";
	};
};

class BadParseException : public std::exception
{
private:
	std::string variable_type;

public:
	const char * what() const throw()
	{
		std::stringstream ss;
		ss << "Parsed stream/string is not of the type: \"" << variable_type << "\"";
		return ss.str().c_str();
	};

	BadParseException(const std::string & type) : variable_type(type) {};
	BadParseException(const char * type) : variable_type(type) {};
};

#endif /* STD_ERRORS_HPP */
