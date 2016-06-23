//Created: 23-05-2014
//Modified: Tue 22 Jul 2014 09:51:56 CEST
//Author: Jonas R. Glesaaen (jonas@glesaaen.com)

#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

namespace hop {

class PMPath;
class PMConfig;

class Collector
{
public:
  virtual void pathCollector(PMPath * path) {};
  virtual void configCollector(PMConfig * object) {};

  virtual ~Collector() {};
};


} //Namespace hop

#endif /* COLLECTOR_HPP */
