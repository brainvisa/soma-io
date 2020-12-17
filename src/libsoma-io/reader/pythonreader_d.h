/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#ifndef SOMAIO_READER_PYTHONREADER_D_H
#define SOMAIO_READER_PYTHONREADER_D_H

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
#include <soma-io/reader/pythonreader.h>
//----------------------------------------------------------------------------

struct soma::PythonReader::Private
{
  Private( const SyntaxSet & r, carto::rc_ptr<soma::DataSource> ds )
  : rules( r ), datasource( ds ), eof( false )
  {}

  Private( const SyntaxSet & r )
  : rules( r ), datasource(), eof( false )
  {}

  SyntaxSet           rules;
  carto::rc_ptr<soma::DataSource>  datasource;
  HelperSet           helpers;
  bool                eof;
};


namespace
{

  template<typename T>
  carto::GenericObject*
  genericHelper( carto::GenericObject*, const std::string &,
                 soma::PythonReader & r )
  {
    r.skipWhile( " \t\n\\\r" );
    soma::DataSource & ds = *r.dataSource();
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    T   x;
    *r.dataSource() >> x;
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    return( new carto::ValueObject<T>( x ) );
  }


  template<>
  carto::GenericObject*
  genericHelper<std::vector<int> >(carto::GenericObject* o, const std::string & s,
                                   soma::PythonReader & r )
  {
    return soma::PythonReader::genericSequenceHelper<std::vector<int> >(
      o, s, r );
  }

  template<>
  carto::GenericObject*
  genericHelper<char>( carto::GenericObject*, const std::string &,
                       soma::PythonReader & r )
  {
    r.skipWhile( " \t\n\\\r" );
    soma::DataSource & ds = *r.dataSource();
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    int x;
    *r.dataSource() >> x;
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    //cout << "number: " << x << std::endl;
    return( new carto::ValueObject<char>( char( x ) ) );
  }


  template<>
  carto::GenericObject*
  genericHelper<unsigned char>( carto::GenericObject*, const std::string &,
                                soma::PythonReader & r )
  {
    r.skipWhile( " \t\n\\\r" );
    soma::DataSource & ds = *r.dataSource();
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    unsigned    x;
    *r.dataSource() >> x;
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    //cout << "number: " << x << std::endl;
    return( new carto::ValueObject<unsigned char>( static_cast<unsigned char>( x ) ) );
  }


  template<>
  carto::GenericObject* 
  genericHelper<std::string>( carto::GenericObject*, const std::string &,
                              soma::PythonReader & r )
  {
    std::string  x;
    char    quote;
    soma::DataSource  & ds = *r.dataSource();
    if( !ds.isOpen() )
      throw carto::eof_error( std::string( "EOF: line" ) + r.lineString(), r.name() );

    r.readWhile( " \t\n\\\r" );
    if( ds.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    quote = ds.getch();
    if( ds.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    if( quote == 'u' ) // unicode string
    {
      quote = ds.getch();
      if( ds.eof() )
        carto::io_error::launchErrnoExcept( ds.url() );
    }
    if( quote != '"' && quote != '\'' )
    {
      std::cerr << "string reader: NOT a string: " << quote << std::endl;
      return 0; // not a string
    }
    unsigned    n = 0;
    x = r.readString( quote, n );
    if( ds.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );

    //cout << "string: " << x << std::endl;
    return( new carto::ValueObject<std::string>( x ) );
  }

  carto::GenericObject* listHelper( carto::GenericObject*, const std::string &,
                                    soma::PythonReader & r )
  {
    // cout << "list\n";
    std::string  id, type;
    soma::DataSource  & ds = *r.dataSource();

    if( !ds.isOpen() )
      return 0;
    r.skipWhile( " \t\n\\\r" );
    if( r.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    char    c = ds.getch(), mark;
    if( ds.eof() )
      carto::io_error::launchErrnoExcept( ds.url() );
    if( c == '(' )
      mark = ')';
    else if( c == '[' )
      mark = ']';
    else
      {
        // cout << "char: " << c << std::endl;
        throw std::runtime_error( std::string( "PythonReader: Not a list/tuple: " )
                             + r.name() + ", line " + r.lineString() );
      }

    carto::ValueObject<carto::ObjectVector> *obj = new carto::ValueObject<carto::ObjectVector>;

    try
      {
        do
          {
            r.skipWhile( " \t\n\\\r" );
            if( r.eof() )
              carto::io_error::launchErrnoExcept( ds.url() );
            c = ds.getch();
            if( ds.eof() )
              carto::io_error::launchErrnoExcept( ds.url() );
            if( c == ',' )  // separator
              {
                r.skipWhile( " \t\n\\\r" );
                if( r.eof() )
                  carto::io_error::launchErrnoExcept( ds.url() );
                c = ds.getch();
                if( ds.eof() )
                  carto::io_error::launchErrnoExcept( ds.url() );
              }
            if( c == mark )
              {
                break;  // end
              }
            ds.ungetch( c );
            obj->getValue().push_back( r.read( obj, "" ) );
          }
        while( ds.isOpen() );

        return( obj );
      }
    catch( std::exception & e )
      {
        delete obj;
        throw;
      }
  }


  carto::GenericObject* noneHelper( carto::GenericObject*, const std::string &,
                                    soma::PythonReader & )
  {
    return 0;
  }


} // namespace (internal linkage)

namespace soma
{

template<typename T>
carto::GenericObject* PythonReader::genericSequenceHelper( carto::GenericObject*,
                                                    const std::string &,
                                                    PythonReader & r )
{
  std::string  id, type;
  DataSource  & ds = *r.dataSource();

  if( !ds.isOpen() )
    return( 0 );
  r.skipWhile( " \t\n\\\r" );
  if( r.eof() )
    carto::io_error::launchErrnoExcept( ds.url() );
  char    c = ds.getch(), mark;
  if( r.eof() )
    carto::io_error::launchErrnoExcept( ds.url() );
  if( c == '(' )
    mark = ')';
  else if( c == '[' )
    mark = ']';
  else
    {
      //cout << "char: " << c << std::endl;
      throw std::runtime_error( std::string( "PythonReader: Not a list/tuple: " )
                            + r.name() + ", line " + r.lineString() );
    }

  carto::ValueObject<T> *obj = new carto::ValueObject<T>;
  carto::GenericObject       *de;
  carto::ValueObject<typename T::value_type> *dev;

  try
    {
      do
        {
          r.skipWhile( " \t\n\\\r" );
          if( r.eof() )
            carto::io_error::launchErrnoExcept( ds.url() );
          c = ds.getch();
          if( ds.eof() )
            carto::io_error::launchErrnoExcept( ds.url() );
          if( c == ',' )  // separator
            {
              r.skipWhile( " \t\n\\\r" );
              if( r.eof() )
                carto::io_error::launchErrnoExcept( ds.url() );
              c = ds.getch();
              if( ds.eof() )
                carto::io_error::launchErrnoExcept( ds.url() );
            }
          if( c == mark )
            {
              break;  // end
            }
          ds.ungetch( c );
          de = genericHelper<typename T::value_type>( obj, "", r );
          dev = dynamic_cast<carto::ValueObject<typename T::value_type>*>( de );
          if( dev )
            obj->getValue().push_back( dev->getValue() );
          else
            {
              delete de;
              throw std::runtime_error( std::string( "PythonReader: wrong type "
                                            "inserted in homogen sequence: "
                                            )
                                    + r.name() + ", line " + r.lineString() );
            }
          delete de;
        }
      while( ds.isOpen() );

      return( obj );
    }
  catch( std::exception & e )
    {
      delete obj;
      throw;
    }
}

template<typename T>
carto::GenericObject* PythonReader::genericDictHelper( carto::GenericObject *,
                                                const std::string &,
                                                PythonReader & r )
{
  std::string                synt;
  DataSource    & ds = *r.dataSource();
  char      c;

  r.skipWhile( " \t\n\\\r" );
  if( r.eof() )
    carto::io_error::launchErrnoExcept( ds.url() );
  c = ds.getch();
  if( ds.eof() )
    carto::io_error::launchErrnoExcept( ds.url() );
  if ( c != '{' ) {
    std::cerr << "Attempt to read a dictionary not starting with '{'" << std::endl;
    return 0;
  }
  r.skipWhile( " \t\n\\\r" );
  if( r.eof() )
    carto::io_error::launchErrnoExcept( ds.url() );

  carto::ValueObject<T>   *obj = new carto::ValueObject<T>;

  try
  {
    r.readDictionary2( *obj );
  }
  catch( std::exception & e )
  {
    delete obj;
    throw;
  }

  return obj;
}

} // namespace soma

#endif

