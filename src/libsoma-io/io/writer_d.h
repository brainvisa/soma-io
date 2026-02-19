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

#ifndef SOMAIO_IO_WRITER_D_H
#define SOMAIO_IO_WRITER_D_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/writer.h>
#include <soma-io/io/formatdictionary.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
#include <soma-io/writer/formatwriter.h>
#include <soma-io/datasource/filedatasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/exception/ioexcept.h>
#include <cartobase/object/property.h>
#include <cartobase/stream/fileutil.h>
//--- system -----------------------------------------------------------------
#include <set>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "WRITER" )
// #define localMsg( message ) std::cout << "WRITER: " << message << std::endl;
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  template <typename T>
  bool GenericWriter::write( const T & obj, carto::Object options )
  {
    Writer<T> *writer = dynamic_cast< Writer<T> * >( this );
    if ( ! writer ) {
      std::string	filename;
      filename = _datasourceinfo->list().dataSource()->url();

      throw carto::format_mismatch_error( filename );
    }
    return writer->write( obj, options );
  }


  template <typename T>
  Writer<T>::Writer() : GenericWriter()
  {
  }


  template <typename T>
  Writer<T>::Writer( carto::rc_ptr<DataSource> ds ) : GenericWriter( ds )
  {
  }


  template <typename T>
  Writer<T>::Writer( const std::string& filename ) : GenericWriter( filename )
  {
  }


  template <typename T>
  Writer<T>::Writer( std::ostream & stream ) : GenericWriter( stream )
  {
  }


  template<typename T>
  Writer<T>::~Writer()
  {
  }


  template<class T>
  std::string Writer<T>::writtenObjectType() const
  {
    return carto::DataTypeCode<T>::name();
  }

  //==========================================================================
  //   W R I T E   M E T H O D S
  //==========================================================================

  //--- useful typedef -------------------------------------------------------
  typedef std::multimap<std::string,std::string> multi_S;
  typedef std::set<std::string> set_S;
  typedef std::pair<std::multimap<std::string, std::string>::const_iterator,
      std::multimap<std::string, std::string>::const_iterator> pair_cit_S;
  //--------------------------------------------------------------------------

  template<class T>
  bool Writer<T>::write( const T & obj, carto::Object options,
                         int passbegin, int passend )
  {
    localMsg( "<" + carto::DataTypeCode<T>::name() + "> " + _datasourceinfo->url() );

    set_S            tried;
    FormatWriter<T>  *writer;
    set_S::iterator  notyet = tried.end();
    int              excp = 0;
    int              exct = -1;
    std::string      excm;

    errno = 0;  // start from a clean situation

    if( !options.get() )
      options = carto::Object::value( carto::PropertySet() );

    //// Reading URI /////////////////////////////////////////////////////////
    std::string filename
      = FileUtil::uriFilename( _datasourceinfo->list().dataSource()->url() );
    carto::Object urioptions
      = FileUtil::uriOptions( _datasourceinfo->list().dataSource()->url() );
    if( urioptions.get() ) {
      _datasourceinfo->list().dataSource()
                             .reset( new FileDataSource( filename ) );
      options->copyProperties( urioptions );
    }

    std::string	format;
    options->getProperty( "format", format );
    localMsg( "format hint: " + format );
    bool exactformat = false;
    try
    {
      carto::Object exact = options->getProperty( "exact_format" );
      exactformat = exact->getScalar();
    }
    catch( ... )
    {
    }

    //// priority to format hint /////////////////////////////////////////////
    if( passbegin <= 1 && passend >= 1 && !format.empty() )
    {
      localMsg( "Pass 1..." );
      writer = FormatDictionary<T>::writeFormat( format );
      if( writer )
      {
        try
        {
          localMsg( "1. try writer " + format );
          if( writer->write( obj, _datasourceinfo, options ) )
          {
            localMsg( "1. " + format + " OK" );
            return true;
          }
          else
          {
            io_error::checkFatalIOErrno( _datasourceinfo->url() );
            if( exactformat )
              throw carto::wrong_format_error( std::string( "Cannot write "
                "object in format " ) + format, _datasourceinfo->url() );
          }
        }
        catch( std::exception & e )
        {
          localMsg( "1. " + format + " failed" );
          if( exactformat )
            throw;
          io_error::checkFatalIOErrno( _datasourceinfo->url() );
          carto::io_error::keepExceptionPriority( e, excp, exct, excm, 5 );
        }
        tried.insert( format );
      }
      else if( exactformat )
        throw carto::wrong_format_error( std::string( "Cannot write "
          "object in format " ) + format, _datasourceinfo->url() );
    }

    std::string	          ext = carto::FileUtil::extension( filename );
    if( ext == "gz" )  // double extension
    {
      std::string ext2 = carto::FileUtil::extension(
        filename.substr( 0, filename.length() - 3 ) );
      if( ext2 != "" )
        ext = ext2 + "." + ext;
    }
    const multi_S	&       extensions = FormatDictionary<T>::writeExtensions();
    pair_cit_S            iext;
    multi_S::const_iterator ie, ee;
    localMsg( "extension: " + carto::toString( ext ) );

    //// try every matching format until one works ///////////////////////////
    if( passbegin <= 2 && passend >= 2 )
    {
      localMsg( "Pass 2..." );
      iext = extensions.equal_range( ext );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
        if( tried.find( ie->second ) == notyet ) {
          writer = FormatDictionary<T>::writeFormat( ie->second );
          if( writer )
          {
            try
            {
              localMsg( "2. try writer " + ie->second );
              if( writer->write( obj, _datasourceinfo, options ) )
              {
                localMsg( "2. " + ie->second + " OK" );
                return true;
              }
              else
              {
                io_error::checkFatalIOErrno( _datasourceinfo->url() );
                if( exactformat )
                  throw carto::wrong_format_error( std::string( "Cannot write "
                    "object in format " ) + format, _datasourceinfo->url() );
              }
            }
            catch( std::exception & e )
            {
              localMsg( " 2. " + ie->second + " failed" );
              if( exactformat )
                throw;
              io_error::checkFatalIOErrno( _datasourceinfo->url() );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
          }
        }
    }

    //// not found or none works: try writers with no extension //////////////
    if( passbegin <= 3 && passend >= 3 && !ext.empty() )
    {
      localMsg( "Pass 3..." );
      iext = extensions.equal_range( "" );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
        if( tried.find( ie->second ) == notyet ) {
          writer = FormatDictionary<T>::writeFormat( ie->second );
          if( writer )
          {
            try
            {
              localMsg( "3. try writer " + ie->second );
              if( writer->write( obj, _datasourceinfo, options ) )
              {
                localMsg( "3. " + ie->second + " OK" );
                return true;
              }
              else
              {
                io_error::checkFatalIOErrno( _datasourceinfo->url() );
                if( exactformat )
                  throw carto::wrong_format_error( std::string( "Cannot write "
                    "object in format " ) + format, _datasourceinfo->url() );
              }
            }
            catch( std::exception & e )
            {
              localMsg( "3. " + ie->second + " failed" );
              if( exactformat )
                throw;
              io_error::checkFatalIOErrno( _datasourceinfo->url() );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
          }
        }
    }

    //// still not found ? well, try EVERY format this time... ///////////////
    if( passbegin <= 4 && passend >= 4 )
    {
      localMsg( "Pass 4..." );
      iext.first = extensions.begin();
      iext.second = extensions.end();
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie )
        if( tried.find( ie->second ) == notyet ) {
          writer = FormatDictionary<T>::writeFormat( ie->second );
          if( writer )
          {
            try
            {
              localMsg( "4. try writer " + ie->second );
              if( writer->write( obj, _datasourceinfo, options ) )
              {
                localMsg( "4. " + ie->second + " OK" );
                return true;
              }
              else
              {
                io_error::checkFatalIOErrno( _datasourceinfo->url() );
                if( exactformat )
                  throw carto::wrong_format_error( std::string( "Cannot write "
                    "object in format " ) + format, _datasourceinfo->url() );
              }
            }
            catch( std::exception & e )
            {
              localMsg( "4. " + ie->second + " failed" );
              if( exactformat )
                throw;
              io_error::checkFatalIOErrno( _datasourceinfo->url() );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
          }
        }
    }

    //// still not succeeded, it's hopeless... ///////////////////////////////
    carto::io_error::launchExcept( exct, excm,
                                   filename + " : no matching format" );
    return false;
  }

}

#undef localMsg
#endif
