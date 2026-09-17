#ifndef CARTOBASE_UUID_UUID_H
#define CARTOBASE_UUID_UUID_H


#include <cartobase/config/cartobase_config.h>
#include <string>


namespace carto
{


  /** Unique identifier class

  UUID is used to generate and to hold unique identifiers. A unique identifier 
  is (almost) sure not to exist anywhere else in the world. Depending on the 
  system, several UUID generation methods may be used (generally mixing date 
  information, hardware system information and perhaps a random part). An 
  UUID is convertible to and from a string so it is very easily to save and 
  reload.

  UUIDs are attached to persistent objects that must be identified and shared 
  between several applications, or on a network, etc. They are a bit like 
  persistent pointers which remain valid across applications.
  */
  class UUID
  {

  public:

    UUID();
    UUID( const UUID& u );
    UUID( const std::string& s );
    bool operator==( const UUID& u ) const;
    std::string toString() const;
    bool isNull() const;
    void generate();

  private:

    void zero();
    typedef unsigned char uuid_type[ 16 ];
    uuid_type data;

  };


} // end namespace carto


#endif
