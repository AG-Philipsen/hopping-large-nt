//Created: 18-08-2014
//Modified: Tue 14 Oct 2014 13:39:34 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef POSITION_DEFAULT_IO_HPP
#define POSITION_DEFAULT_IO_HPP

#include"position_io.hpp"
#include"position_class.hpp"
#include<boost/lexical_cast.hpp>

namespace Position {

class DefaultPrinter : public Printer
{
private:
	std::ostream & os;

public:
	virtual void print(const pos & p)
	{
		os << "{";

    size_t size = p.size();

    if(size == 0) {
      os << "}";
      return;
    }

    os << p.at(0);

    for(int i = 1; i < size; ++i)
      os << "," << p.at(i);

		os << "}";
	};

	explicit DefaultPrinter(std::ostream & out = std::cout) : os(out) {};
	virtual ~DefaultPrinter() {};
};

class SymbolPrinter : public Printer
{
private: 
  std::ostream & os;
  char variable;

public:
  virtual void print(const pos & p)
  {
    int i_char = (int)'i';
    os << variable;

    for(int i = 0; i < p.size(); ++i) {

      int val = p.at(i);

      if(val == 0)
        continue;

      os << ( (val > 0) ? " + " : " - " );

      if(abs(val) != 1)
        os << abs(val) << '*';

      os << (char)(i_char + i);
    }
  };

  explicit SymbolPrinter(std::ostream & os = std::cout, char var_name = 'x') 
    : os(os),
      variable(var_name)
  {};
  virtual ~SymbolPrinter() {};
};

class DefaultParser : public Parser
{
public:
	virtual pos parseString(std::string pos_str)
	{
		if(pos_str.empty() or pos_str.front() != '{' or pos_str.back() != '}')
			throw BadParseException("Position");

		pos_str.erase(pos_str.begin()); pos_str.pop_back();

		if(pos_str.empty())
			return pos();

		int elements = std::count(pos_str.begin(), pos_str.end(), ',') + 1;
		pos result(elements);

		std::istringstream iss(pos_str);
		std::string number;

		int i = 0;

		while(std::getline(iss, number, ',')) {

			try{
				result[i] = boost::lexical_cast<int>(number);
			} catch(boost::bad_lexical_cast &) {
				throw BadParseException("Position");
			}
			
			++i;
		}

		return result;
	};

	virtual ~DefaultParser() {};
};

} //Namespace Position


#endif /* POSITION_DEFAULT_IO_HPP */
