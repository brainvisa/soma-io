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

#ifndef SOMAIO_IO_READER_D_H
#define SOMAIO_IO_READER_D_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>              // format info
#include <soma-io/datasourceinfo/datasourceinfoloader.h>       // format check
#include <soma-io/datasource/datasourcelist.h>                       // member
#include <soma-io/datasource/datasource.h>        // useful for none() pointer
#include <soma-io/datasource/filedatasource.h>          // used by constructor
#include <soma-io/datasource/streamdatasource.h>        // used by constructor
#include <soma-io/io/formatdictionary.h>             // used by read() method
#include <soma-io/io/reader.h>                            // class declaration
#include <soma-io/reader/formatreader.h>              // used by read() method
#include <soma-io/allocator/allocator.h>             // allocator management
//--- cartobase --------------------------------------------------------------
#include <cartobase/exception/ioexcept.h>                  // launch exception
#include <cartobase/object/object.h>                       // header & options
#include <cartobase/object/property.h>                     // header & options
#include <cartobase/stream/fileutil.h>               // finding file extension
//--- system -----------------------------------------------------------------
#include <set>
#include <map>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "READER" )
// localMsg must be undef at end of file

//----------------------------------------------------------------------------

namespace soma
{
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template<class T> Reader<T>::Reader()
    : _datasourceinfo( new DataSourceInfo )
  {
  }

  template<class T> Reader<T>::Reader( carto::rc_ptr<DataSource> ds )
    : _datasourceinfo( new DataSourceInfo( ds ) )
  {
  }

  template<class T> Reader<T>::Reader( carto::rc_ptr<DataSourceInfo> dsi )
    : _datasourceinfo( new DataSourceInfo( *dsi ) )
  {
  }

  template<class T> Reader<T>::Reader( const std::string& filename )
  : _datasourceinfo( new DataSourceInfo(
                  carto::rc_ptr<DataSource>( new FileDataSource( filename ) )
                   ) )
  {
  }

  template<class T> Reader<T>::Reader( std::istream & stream )
  : _datasourceinfo( new DataSourceInfo(
                  carto::rc_ptr<DataSource>( new IStreamDataSource( stream ) )
                   ) )
  {
  }

  template<class T> Reader<T>::~Reader()
  {
  }

  //==========================================================================
  //   A L L O C A T O R
  //==========================================================================
  template <typename T>
  void Reader<T>::setAllocatorContext( const AllocatorContext & ac )
  {
    _alloccontext = ac;
  }

  template <typename T>
  const AllocatorContext & Reader<T>::allocatorContext() const
  {
    return _alloccontext;
  }

  //==========================================================================
  //   O P T I O N S
  //==========================================================================
  template<class T>
  void Reader<T>::setOptions( carto::Object options )
  {
    _options = options;
  }

  template<class T>
  carto::Object Reader<T>::options() const
  {
    return _options;
  }

  template<class T>
  carto::Object & Reader<T>::options()
  {
    return _options;
  }

  //==========================================================================
  //   D A T A S O U R C E
  //==========================================================================
  template<class T>
  void Reader<T>::attach( carto::rc_ptr<DataSource> ds )
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      _datasourceinfo->list().dataSource() = ds;
    } else {
      _datasourceinfo->list().addDataSource( "default", ds );
    }
  }

  template<class T>
  void Reader<T>::attach( const std::string & filename, offset_t offset )
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      _datasourceinfo->list().dataSource()
        = carto::rc_ptr<DataSource>( new FileDataSource( filename, offset ) );
    } else {
      _datasourceinfo->list().addDataSource( "default",
        carto::rc_ptr<DataSource>( new FileDataSource( filename, offset ) ) );
    }
  }

  template<class T>
  void Reader<T>::attach( std::istream & stream )
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      _datasourceinfo->list().dataSource()
          = carto::rc_ptr<DataSource>( new IStreamDataSource( stream ) );
    } else {
      _datasourceinfo->list().addDataSource( "default",
          carto::rc_ptr<DataSource>( new IStreamDataSource( stream ) ) );
    }
  }

  template<class T>
  const carto::rc_ptr<DataSource> Reader<T>::dataSource() const
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      return _datasourceinfo->list().dataSource();
    } else {
      return DataSource::none();
    }
  }

  template<class T>
  carto::rc_ptr<DataSource> Reader<T>::dataSource()
  {
    if ( !_datasourceinfo->list().empty() ) {
      return _datasourceinfo->list().dataSource();
    } else {
      return DataSource::none();
    }
  }

  template <typename T>
  void Reader<T>::flush()
  {
    if( _datasourceinfo->list().dataSource() )
      _datasourceinfo->list().dataSource()->flush();
  }


  template <typename T>
  void Reader<T>::close()
  {
    if( _datasourceinfo->list().dataSource() )
      _datasourceinfo->list().dataSource()->close();
  }

  //==========================================================================
  //   R E A D   M E T H O D S
  //==========================================================================

  //--- useful typedef -------------------------------------------------------
  typedef std::multimap<std::string,std::string> multi_S;
  typedef std::set<std::string> set_S;
  typedef std::pair<std::multimap<std::string, std::string>::const_iterator,
      std::multimap<std::string, std::string>::const_iterator> pair_cit_S;
  //--------------------------------------------------------------------------

  template<class T>
  bool Reader<T>::read( T & obj, carto::Object header,
                        int passbegin, int passend )
  {
    localMsg( "<" + carto::DataTypeCode<T>::name() + "> "
      + _datasourceinfo->url() );

    if( !header.isNone() )
      _datasourceinfo->header() = header;

    if( !dataSource() )
      throw std::runtime_error( "Reader with no source of data" );
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );

    //// Reading URI /////////////////////////////////////////////////////////
    std::string uri = _datasourceinfo->list().dataSource()->url();
    std::string filename = FileUtil::uriFilename( uri );
    carto::Object urioptions = FileUtil::uriOptions( uri );
    if( urioptions.get() ) {
      _datasourceinfo->list().dataSource()
                             .reset( new FileDataSource( filename ) );
      _options->copyProperties( urioptions );
    }

    //// Checking format /////////////////////////////////////////////////////
    DataSourceInfoLoader  dsil; // manages the case of a not-none header
    DataSourceInfo        dsi = dsil.check(
      *_datasourceinfo, _options,
      ( passbegin < 2 ? 1 : passbegin - 1 ), passend - 1 );
    if( dsi.list().empty() )
      dsil.launchException();
    if( !dsi.header().get() )
      dsil.launchException();
    *_datasourceinfo = dsi;
    try {
      if( _options->getProperty( "partial_reading" )->getScalar() )
        /* disable mmap here because partial reading on an already allocated
           volume is not compatible with mmap */
        _datasourceinfo->capabilities().setMemoryMapping( false );
    } catch( ... ) {
    }
    _alloccontext.setDataSourceInfo( _datasourceinfo );

    std::string	format;
    if( !_options->getProperty( "format", format )
        && !_datasourceinfo->header()->getProperty( "format", format ) )
      _datasourceinfo->header()->getProperty( "file_type", format );
    localMsg( "format: " + format + " for file: " + uri );

    //// Reading data ////////////////////////////////////////////////////////
    set_S                        tried;
    std::set<FormatReader<T> *>  triedf;
    FormatReader<T>              *reader;
    std::auto_ptr<FormatReader<T> >   readerc;
    set_S::iterator	             notyet = tried.end();
    typename std::set<FormatReader<T> *>::iterator	notyetf = triedf.end();
    int          excp = 0;
    int          exct = -1;
    std::string  excm;

    //// Pass1 : priority to format hint /////////////////////////////////////
    if( passbegin <= 1 && passend >=1 && !format.empty() )
    {
      reader = FormatDictionary<T>::readFormat( format );
      if( reader )
      {
        try {
          localMsg( "1. try reader " + format );
          readerc.reset( reader->clone() );
          readerc->setupAndRead( obj, _datasourceinfo,
                                _alloccontext, _options );
          localMsg( "1. " + format + " OK" );
          return true;
        }
        catch( std::exception & e ) {
          localMsg( "1. " + format + " failed" );
          carto::io_error::keepExceptionPriority( e, excp, exct, excm, 5 );
        }
        tried.insert( format );
        triedf.insert( reader );
      }
    }

    std::string	ext = carto::FileUtil::extension( filename );
    const multi_S	& extensions = FormatDictionary<T>::readExtensions();
    pair_cit_S	             iext;
    multi_S::const_iterator  ie, ee;

    //// Pass 2 : try every matching format until one works //////////////////
    if( passbegin <= 2 && passend >=2 )
    {
      iext = extensions.equal_range( ext );
      for( ie=iext.first, ee = iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet )
        {
          reader = FormatDictionary<T>::readFormat( ie->second );
          if( reader && triedf.find( reader ) == notyetf ) {
            try {
              localMsg( "2. try reader " + ie->second );
              readerc.reset( reader->clone() );
              readerc->setupAndRead( obj, _datasourceinfo,
                                    _alloccontext, _options );
              localMsg( "2. " + ie->second + " OK" );
              return true;
            }
            catch( std::exception & e ) {
              localMsg( "2. " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 3 : not found or none works: try readers with no extension /////
    if( passbegin <= 3 && passend >= 3 && !ext.empty())
    {
      iext = extensions.equal_range( "" );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( ie->second );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try {
              localMsg( "3. try reader " + ie->second );
              readerc.reset( reader->clone() );
              readerc->setupAndRead( obj, _datasourceinfo,
                                    _alloccontext, _options );
              localMsg( "3. " + ie->second + " OK" );
              return true;
            }
            catch( std::exception & e ) {
              localMsg( "3. " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 4 : still not found ? well, try EVERY format this time... //////
    if( passbegin <= 4 && passend >= 4 )
    {
      iext.first = extensions.begin();
      iext.second = extensions.end();
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( ie->second );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try {
              localMsg( "4. try reader " + ie->second );
              readerc.reset( reader->clone() );
              readerc->setupAndRead( obj, _datasourceinfo,
                                    _alloccontext, _options );
              localMsg( "4. " + ie->second + " OK" );
              return true;
            }
            catch( std::exception & e ) {
              localMsg( "4. " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// End : still not succeeded, it's hopeless... /////////////////////////
    carto::io_error::launchExcept( exct, excm,
                                   filename + " : no matching format" );
    return false;
  }

  template<class T>
  T* Reader<T>::read( carto::Object header, int passbegin, int passend )
  {
    localMsg( "<" + carto::DataTypeCode<T>::name() + "> "
      + _datasourceinfo->url() );

    if( !header.isNone() )
      _datasourceinfo->header() = header;

    if( !dataSource() )
      throw std::runtime_error( "Reader with no source of data" );
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );

    //// Reading URI /////////////////////////////////////////////////////////
    std::string uri = _datasourceinfo->list().dataSource()->url();
    std::string filename = FileUtil::uriFilename( uri );
    carto::Object urioptions = FileUtil::uriOptions( uri );
    if( urioptions.get() ) {
      _datasourceinfo->list().dataSource()
                             .reset( new FileDataSource( filename ) );
      _options->copyProperties( urioptions );
    }

    //// Checking format /////////////////////////////////////////////////////
    DataSourceInfoLoader  dsil; // manages the case of a not-none header
    DataSourceInfo        dsi = dsil.check( *_datasourceinfo, _options );
    if( dsi.list().empty() )
      dsil.launchException();
    if( dsi.header().get() )
      dsil.launchException();
    *_datasourceinfo = dsi;
    _alloccontext.setDataSourceInfo( _datasourceinfo );

    std::string format;
    if( !_options->getProperty( "format", format )
        && !_datasourceinfo->header()->getProperty( "format", format ) )
      _datasourceinfo->header()->getProperty( "file_type", format );

    localMsg( "format: " + format + " for file: " + uri );

    //// Reading data ////////////////////////////////////////////////////////
    set_S                         tried;
    std::set<FormatReader<T> *>   triedf;
    FormatReader<T>               *reader;
    std::auto_ptr<FormatReader<T> >    readerc;
    set_S::iterator               notyet = tried.end();
    typename std::set<FormatReader<T> *>::iterator  notyetf = triedf.end();
    T            *obj;
    int          excp = 0;
    int          exct = -1;
    std::string  excm;

    //// Pass 1 : prioroty to format hint ////////////////////////////////////
    if( passbegin <= 1 && passend >= 1 && !format.empty() )	{
      reader = FormatDictionary<T>::readFormat( format );
      if( reader ) {
        try {
          localMsg( "1. try reader " + format );
          readerc.reset( reader->clone() );
          obj = readerc->createAndRead( _datasourceinfo,
                                       _alloccontext, _options );
          if( obj ) {
            localMsg( "1. " + format + " OK" );
            return obj;
          }
	      } catch( std::exception & e ) {
          localMsg( "1. " + format + " failed" );
          carto::io_error::keepExceptionPriority( e, excp, exct, excm, 5 );
	      }
        tried.insert( format );
        triedf.insert( reader );
      }
    }

    std::string	ext = carto::FileUtil::extension( filename );

    const multi_S &  extensions = FormatDictionary<T>::readExtensions();

    pair_cit_S               iext;
    multi_S::const_iterator  ie, ee;

    //// Pass 2 : try every matching format until one works //////////////////
    if( passbegin <= 2 && passend >= 2 )
    {
      iext = extensions.equal_range( ext );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( (*ie).second );
          if( reader && triedf.find( reader ) == notyetf ) {
            try {
              localMsg( "2. try reader " + ie->second );
              readerc.reset( reader->clone() );
              obj = readerc->createAndRead( _datasourceinfo,
                                          _alloccontext, _options );
              if( obj ) {
                localMsg( "2. " + ie->second + " OK" );
                return obj;
              }
            } catch( std::exception & e ) {
              localMsg( "2. " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 3 : not found or none works: try readers with no extension /////
    if( passbegin <= 3 && passend >= 3 && !ext.empty() )
    {
      iext = extensions.equal_range( "" );
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( (*ie).second );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try {
              localMsg( "3. try reader " + ie->second );
              readerc.reset( reader->clone() );
              obj = readerc->createAndRead( _datasourceinfo,
                                           _alloccontext, _options );
              if( obj ) {
                localMsg( "3. " + ie->second + " OK" );
                return obj;
              }
            } catch( std::exception & e ) {
              localMsg( "3.  " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 4 : still not found ? well, try EVERY format this time... //////
    if( passbegin <= 4 && passend >= 4 )
    {
      iext.first = extensions.begin();
      iext.second = extensions.end();
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( ie->second );
          if( reader && triedf.find( reader ) == notyetf )
          {
            try {
              localMsg( "4. try reader " + ie->second );
              readerc.reset( reader->clone() );
              obj = readerc->createAndRead( _datasourceinfo,
                                          _alloccontext, _options );
              if( obj ) {
                localMsg( "4. " + ie->second + " OK" );
                return obj;
              }
            } catch( std::exception & e ) {
              localMsg( "4. " + ie->second + " failed" );
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// End : still not succeeded, it's hopeless... /////////////////////////
    carto::io_error::launchExcept( exct, excm,
                                   filename + " : no matching format" );
    return 0;
  }

}

#undef localMsg
#endif
