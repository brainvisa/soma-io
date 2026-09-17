#ifndef SOMAIO_IO_FORMATDICTIONARY_D_H
#define SOMAIO_IO_FORMATDICTIONARY_D_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/formatdictionary.h>                  // class declaration
#include <soma-io/reader/formatreader.h>                             // map of
#include <soma-io/writer/formatwriter.h>                             // map of
//--- cartobase --------------------------------------------------------------
#include <cartobase/type/types.h> // where is it used ?
//--- system -----------------------------------------------------------------
#include <string>
#include <map>
#include <set>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "FORMATDICTIONARY" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  template <typename T> 
  std::map<std::string, FormatReader<T>*> & 
  FormatDictionary<T>::_readformats()
  {
    static std::map<std::string, FormatReader<T>*> form;
    return form;
  }

  template <typename T> 
  std::map<std::string, FormatWriter<T>*> & 
  FormatDictionary<T>::_writeformats()
  {
    static std::map<std::string, FormatWriter<T>*> form;
    return form;
  }

  template <typename T> 
  std::multimap<std::string, std::string> & 
  FormatDictionary<T>::_readextensions()
  {
    static std::multimap<std::string, std::string>	ext;
    return ext;
  }

  template <typename T> 
  std::multimap<std::string, std::string> & 
  FormatDictionary<T>::_writeextensions()
  {
    static std::multimap<std::string, std::string>	ext;
    return ext;
  }

  template <typename T>
  const std::multimap<std::string, std::string> & 
  FormatDictionary<T>::readExtensions()
  {
    init();
    return _readextensions();
  }

  template <typename T>
  const std::multimap<std::string, std::string> & 
  FormatDictionary<T>::writeExtensions()
  {
    init();
    return _writeextensions();
  }

  template <typename T>
  void FormatDictionary<T>::init()
  {
    static bool initialized = false;
    if( !initialized )
      {
//         localMsg("initializing soma format dictionaries ...");
//         localMsg("read formats dictionary [" + carto::toString(&(_readformats())) + "] ...")
//         localMsg("write formats dictionary [" + carto::toString(&(_writeformats())) + "] ...")
        initialized = true;
        carto::DataTypeCode<T>	dtc;
        IOObjectTypesDictionary::registerReadType( dtc.name(), 
                                                   &readFormats );
        IOObjectTypesDictionary::registerWriteType( dtc.name(), 
                                                    &writeFormats );
        registerBaseFormats();
      }
  }


  template <typename T>
  void FormatDictionary<T>::registerBaseFormats()
  {
  }


  template <typename T> 
  void 
  FormatDictionary<T>::registerFormat( const std::string & format, 
                                       FormatReader<T>* formatObj, 
                                       const std::vector<std::string> 
                                       & extensions )
  {
    init();

    FormatReader<T>	*oldr = readFormat( format );
    delete oldr;
    _readformats()[ format ] = formatObj;

    std::vector<std::string>::const_iterator	ie, ee = extensions.end();
    for( ie=extensions.begin(); ie!=ee; ++ie )
      _readextensions().insert
        ( std::pair<std::string, std::string>( *ie, format ) );
  }


  template <typename T> 
  void 
  FormatDictionary<T>::registerFormat( const std::string & format, 
                                       FormatWriter<T>* formatObj, 
                                       const std::vector<std::string> 
                                       & extensions )
  {
    init();

    FormatWriter<T>	*oldr = writeFormat( format );
    delete oldr;
    _writeformats()[ format ] = formatObj;

    std::vector<std::string>::const_iterator	ie, ee = extensions.end();
    for( ie=extensions.begin(); ie!=ee; ++ie )
      _writeextensions().insert
        ( std::pair<std::string, std::string>( *ie, format ) );
  }


  template <typename T> 
  FormatReader<T> * 
  FormatDictionary<T>::readFormat( const std::string & format )
  {
    init();

    typename std::map<std::string, FormatReader<T>*>::const_iterator	i 
      = _readformats().find( format );
    if( i == _readformats().end() )
      return( 0 );
    return( (*i).second );
  }


  template <typename T> 
  FormatWriter<T> * 
  FormatDictionary<T>::writeFormat( const std::string & format )
  {
    init();

    typename std::map<std::string, FormatWriter<T>*>::const_iterator	i 
      = _writeformats().find( format );
    if( i == _writeformats().end() )
      return( 0 );
    return( (*i).second );
  }


  template <typename T> 
  std::set<std::string> FormatDictionary<T>::readFormats()
  {
//     localMsg("Retrieving soma read formats from format dictionary [" + carto::toString(&(_readformats())) + "] ...")
    std::set<std::string>	f;
    typename std::map<std::string, FormatReader<T>*>::const_iterator 
      i, e = _readformats().end();
    for( i=_readformats().begin(); i!=e; ++i )
      f.insert( i->first );
    return( f );
  }


  template <typename T> 
  std::set<std::string> FormatDictionary<T>::writeFormats()
  {
//     localMsg("Retrieving soma write formats from format dictionary [" + carto::toString(&(_writeformats())) + "] ...")
    std::set<std::string>	f;
    typename std::map<std::string, FormatWriter<T>*>::const_iterator 
      i, e = _writeformats().end();
    for( i=_writeformats().begin(); i!=e; ++i )
      f.insert( i->first );
    return( f );
  }

}

#undef localMsg

#endif
