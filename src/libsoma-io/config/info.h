#ifndef SOMAIO_CONFIG_INFO_H
#define SOMAIO_CONFIG_INFO_H

#include <string>
#include <iostream>
#include <vector>

namespace carto
{

  /**	Information on Cartograph libraries and extensions */
  class Info
  {
  public:
    Info();
    virtual ~Info();
    static std::vector<Info*> & infos();

    static void print( std::ostream & output = std::cout );
    /** inherited classes can overload this function (it doesn't do anything 
	in the base class) */
    virtual void printPaths( std::ostream & output = std::cout );
    /** inherited classes can overload this function (it doesn't do anything 
	in the base class) */
    virtual void printBuiltins( std::ostream & output = std::cout );
    /** inherited classes can overload this function (it doesn't do anything 
	in the base class) */
    virtual void printOtherInfo( std::ostream & output = std::cout );
    static void printVersions( std::ostream & output = std::cout );
    /// inherited classes can overload this function
    virtual void printVersion( std::ostream & output = std::cout );
  };


  /// CartoBase library info
  class CartoBaseInfo : public Info
  {
  public:
    CartoBaseInfo();
    virtual ~CartoBaseInfo();
    virtual void printVersion( std::ostream & output = std::cout );
  };

}

#endif

