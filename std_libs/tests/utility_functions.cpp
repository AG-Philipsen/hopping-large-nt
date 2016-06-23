/*
 * Created: 02-10-2014
 * Modified: Thu 02 Oct 2014 13:13:21 CEST
 * Author: Jonas R. Glesaaen (jonas@glesaaen.com)
 */

#include"utility_functions.hpp"
#include<fstream>
#include<sstream>
#include<stdexcept>
#include<boost/lexical_cast.hpp>

namespace UnitTest {

std::list< std::vector<int> > ParsePermData(std::string filename)
{
  auto parsed_data = std::list< std::vector<int> >{};
  auto line = std::string{};

  std::ifstream ifs(filename);

  if(!ifs)
    throw std::runtime_error("Unable to open permutation config file");

  while( std::getline(ifs,line) )
  {
    std::istringstream iss(line);
    std::string number;

    auto config_vec = std::vector<int>{};
    config_vec.reserve(6);

    while( std::getline(iss, number, ',') )
    {
      config_vec.push_back( boost::lexical_cast<int>(number) );
    }

    parsed_data.push_back(std::move(config_vec));
  }

  return parsed_data;
}

} //Namespace UnitTest
