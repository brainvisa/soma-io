#include <soma-io/config/info.h>
#include <soma-io/plugin/plugin.h>
#include <soma-io/io/formatdictionary.h>
#include <cartobase/config/version.h>
#include <algorithm>

using namespace carto;
using namespace std;


vector<Info*> & Info::infos()
{
  static bool initialized = false;
  static vector<Info*>	inf;
  if( !initialized )
    {
      initialized = true;
      new CartoBaseInfo;
    }
  return inf;
}


Info::Info()
{
  infos().push_back( this );
}


Info::~Info()
{
  vector<Info*>	& inf = infos();
  vector<Info*>::iterator 
    i = find( inf.begin(), inf.end(), this );
  if( i != inf.end() )
    inf.erase( i );
}


void Info::printPaths( ostream & )
{
}


void Info::printBuiltins( ostream & )
{
}


void Info::printOtherInfo( ostream & )
{
}


void Info::printVersion( ostream & )
{
}


void Info::printVersions( ostream & out )
{
  vector<Info*>::iterator	i, n = infos().end();
  for( i=infos().begin(); i!=n; ++i )
    (*i)->printVersion( out );
}


void Info::print( ostream & out )
{
  printVersions( out );
  out << "General information:" << endl;
  vector<Info*>::iterator	i, n = infos().end();
  for( i=infos().begin(); i!=n; ++i )
    (*i)->printPaths( out );
  out << "- Compiled with the following built-in options:" << endl;
  for( i=infos().begin(); i!=n; ++i )
    (*i)->printBuiltins( out );
  out << "Plugin support    : ";
#ifndef CARTO_NO_DLOPEN
  out << "yes" << endl;
#else
  out << "no" << endl;
#endif
  out << "Plugins:" << endl;
  set<Plugin*>		pl = PluginManager::singleton().plugins();
  if( pl.empty() )
    out << "(none)" << endl;
  else
    {
      set<Plugin*>::iterator	ip, ep = pl.end();
      unsigned			i = 0;
      for( ip=pl.begin(); ip!=ep; ++ip, ++i )
	out << i << "\t" << (*ip)->name() << endl;
    }
  out << "- IO types and formats:" << endl;
  out << "  Input formats:" << endl;
  map<string, IOObjectTypesDictionary::FormatInfo> 
    & types = IOObjectTypesDictionary::readTypes();
  if( types.empty() )
    out << "(none, there is obviously a problem !)\n";
  else
    {
      map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
	ifo, efo = types.end();
      set<string>::iterator				ifm, efm;
      for( ifo=types.begin(); ifo!=efo; ++ifo )
        {
          out << "Data type: " << ifo->first << " - formats : ";
          set<string>	formats = ifo->second();
          if( formats.empty() )
            out << "(none)" << endl;
          else
            {
              for( ifm=formats.begin(), efm=formats.end(); ifm!=efm; ++ifm )
                out << *ifm << " ";
              out << endl;
            }
        }
    }

  out << endl;
  out << "  Output formats:" << endl;
  map<string, IOObjectTypesDictionary::FormatInfo> 
    & otypes = IOObjectTypesDictionary::writeTypes();
  if( otypes.empty() )
    out << "(none, there is obviously a problem !)\n";
  else
    {
      map<string, IOObjectTypesDictionary::FormatInfo>::iterator 
	ifo, efo = otypes.end();
      set<string>::iterator				ifm, efm;
      for( ifo=otypes.begin(); ifo!=efo; ++ifo )
	  {
	    out << "Data type: " << ifo->first << " - formats : ";
	    set<string>	formats = ifo->second();
	    if( formats.empty() )
	      out << "(none)" << endl;
	    else
	      {
		for( ifm=formats.begin(), efm=formats.end(); ifm!=efm; ++ifm )
		  out << *ifm << " ";
		out << endl;
	      }
	  }
    }
  out << endl;

  for( i=infos().begin(); i!=n; ++i )
    (*i)->printOtherInfo( out );
}


CartoBaseInfo::CartoBaseInfo()
  : Info()
{
}


CartoBaseInfo::~CartoBaseInfo()
{
}


void CartoBaseInfo::printVersion( ostream & out )
{
  out << "CartoBase version : " << cartobaseVersionString() << endl;
}


