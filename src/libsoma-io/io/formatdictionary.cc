//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/formatdictionary.h>
//--- system -----------------------------------------------------------------
#include <string>
#include <map>
#include <set>
#include <vector>
//----------------------------------------------------------------------------

using namespace soma;
using namespace std;

void IOObjectTypesDictionary::registerReadType( const string & datatype, 
                                                FormatInfo info )
{
  readTypes()[ datatype ] = info;
}


void IOObjectTypesDictionary::registerWriteType( const string & datatype, 
                                                 FormatInfo info )
{
  writeTypes()[ datatype ] = info;
}


map<string, IOObjectTypesDictionary::FormatInfo> & 
IOObjectTypesDictionary::readTypes()
{
  static map<string, FormatInfo>	t;
  return t;
}


map<string, IOObjectTypesDictionary::FormatInfo> & 
IOObjectTypesDictionary::writeTypes()
{
  static map<string, FormatInfo>	t;
  return t;
}


bool IOObjectTypesDictionary::hasReadType( const string & datatype )
{
  map<string, IOObjectTypesDictionary::FormatInfo> & t 
    = readTypes();
  map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
    i = t.find( datatype );
  return( i != t.end() );
}


bool IOObjectTypesDictionary::hasWriteType( const string & datatype )
{
  map<string, IOObjectTypesDictionary::FormatInfo> & t 
    = writeTypes();
  map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
    i = t.find( datatype );
  return( i != t.end() );
}

