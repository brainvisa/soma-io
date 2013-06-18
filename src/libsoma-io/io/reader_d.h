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
//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>        // read(): format info
#include <soma-io/datasourceinfo/datasourceinfoloader.h> // read(): format check
#include <soma-io/datasource/datasourcelist.h>                         // member
#include <soma-io/datasource/datasource.h>          // useful for none() pointer
#include <soma-io/datasource/filedatasource.h>            // used by constructor
#include <soma-io/datasource/streamdatasource.h>          // used by constructor
#include <soma-io/io/formatdictionary.h>               // used by read() method
#include <soma-io/io/reader.h>                              // class declaration
#include <soma-io/reader/formatreader.h>                // used by read() method
#include <soma-io/allocator/allocator.h>               // allocator management
//--- cartobase ----------------------------------------------------------------
#include <cartobase/exception/ioexcept.h>                    // launch exception
#include <cartobase/object/object.h>                         // header & options
#include <cartobase/object/property.h>                       // header & options
#include <cartobase/stream/fileutil.h>                 // finding file extension
#include <cartobase/config/verbose.h>                         // verbosity level
//--- system -------------------------------------------------------------------
#include <set>
#include <map>
#include <iostream>                                                     // debug
//------------------------------------------------------------------------------

namespace soma
{
  //============================================================================
  //   C O N S T R U C T O R S
  //============================================================================
  template<class T> Reader<T>::Reader()
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
  
  //============================================================================
  //   A L L O C A T O R
  //============================================================================
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

  //============================================================================
  //   O P T I O N S
  //============================================================================
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

  //============================================================================
  //   D A T A S O U R C E
  //============================================================================
  template<class T>
  void Reader<T>::attach( carto::rc_ptr<DataSource> ds )
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      _datasourceinfo->list().dataSource( "default", 0 ) = ds;
    } else {
      _datasourceinfo->list().addDataSource( "default", ds );
    }
  }

  template<class T>
  void Reader<T>::attach( const std::string & filename, offset_t offset )
  {
    if ( !_datasourceinfo->list().empty( "default" ) ) {
      _datasourceinfo->list().dataSource( "default", 0 )
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
      _datasourceinfo->list().dataSource( "default", 0 ) 
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
      return _datasourceinfo->list().dataSource( "default", 0 );
    } else {
      return DataSource::none();
    }
  }

  template<class T>
  carto::rc_ptr<DataSource> Reader<T>::dataSource()
  {
    if ( !_datasourceinfo->list().empty() ) {
      return _datasourceinfo->list().dataSource( "default", 0 );
    } else {
      return DataSource::none();
    }
  }
  
  template <typename T>
  void Reader<T>::flush()
  {
    if( _datasourceinfo->list().dataSource( "default", 0 ) )
      _datasourceinfo->list().dataSource( "default", 0 )->flush();
  }


  template <typename T>
  void Reader<T>::close()
  {
    if( _datasourceinfo->list().dataSource( "default", 0 ) )
      _datasourceinfo->list().dataSource( "default", 0 )->close();
  }
  
  //============================================================================
  //   R E A D   M E T H O D S
  //============================================================================
  
  //--- useful typedef ---------------------------------------------------------
  typedef std::multimap<std::string,std::string> multi_S;
  typedef std::set<std::string> set_S;
  typedef std::pair<std::multimap<std::string, std::string>::const_iterator, 
      std::multimap<std::string, std::string>::const_iterator> pair_cit_S;
  //----------------------------------------------------------------------------
  
  template<class T>
  bool Reader<T>::read( T & obj, carto::Object header )
  {
    if( carto::debugMessageLevel > 3 ) {
      std::cout << "READER:: <" << carto::DataTypeCode<T>::name() << ">\n";
    }
    
    if( !header.isNone() )
      _datasourceinfo->header() = header;

    if( !dataSource() )
      throw std::runtime_error( "Reader with no source of data" );
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );
    
    //// Checking format ///////////////////////////////////////////////////////
    DataSourceInfoLoader  dsil; // manages the case of a not-none header
    DataSourceInfo        dsi = dsil.check( *_datasourceinfo, _options );
    if( dsi.list().empty() )
      dsil.launchException();
    if( !dsi.header().get() )
      dsil.launchException();
    *_datasourceinfo = dsi;
    try {
      if( (bool)_options->getProperty( "partial_reading" )->getScalar() )
        _datasourceinfo->capabilities().setMemoryMapping( false );
    } catch( ... ) {
    }
    _alloccontext.setDataSourceInfo( _datasourceinfo );
    
    std::string	format;
    if( !_options->getProperty( "format", format )  // user options first
        && !_datasourceinfo->header()->getProperty( "format", format ) ) // new style
      _datasourceinfo->header()->getProperty( "file_type", format ); // old style
    
    if( carto::debugMessageLevel > 3 ) {
      std::cout << "READER:: format: " << format << std::endl;
    }
    
    //// Reading data //////////////////////////////////////////////////////////
    std::string   filename  = _datasourceinfo->list()
                                             .dataSource( "default", 0 )
                                             ->url();
    set_S                        tried;
    std::set<FormatReader<T> *>  triedf;
    FormatReader<T>              *reader;
    set_S::iterator	             notyet = tried.end();
    typename std::set<FormatReader<T> *>::iterator	notyetf = triedf.end();
    int					 excp = 0;
    int					 exct = -1;
    std::string	 excm;

    //// Pass1 : priority to format hint ///////////////////////////////////////
    if( !format.empty() ) {
      reader = FormatDictionary<T>::readFormat( format );
      if( reader ) {
        try {
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 1. try reader " << format << std::endl;
          }
          reader->setupAndRead( obj, _datasourceinfo, _alloccontext, _options );
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 1. " << format << " OK\n";
          }
          return true;
	      } catch( std::exception & e ) {
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 1. failed\n";
          }
          carto::io_error::keepExceptionPriority( e, excp, exct, excm, 5 );
	      }
	      tried.insert( format );
        triedf.insert( reader );
      }
    }

    std::string	ext = carto::FileUtil::extension( filename );

    const multi_S	& extensions = FormatDictionary<T>::readExtensions();

    pair_cit_S	             iext = extensions.equal_range( ext );
    multi_S::const_iterator  ie, ee = iext.second;

    //// Pass 2 : try every matching format until one works ////////////////////
    for( ie=iext.first; ie!=ee; ++ie ) {
      if( tried.find( (*ie).second ) == notyet ) {
        reader = FormatDictionary<T>::readFormat( ie->second );
        if( reader && triedf.find( reader ) == notyetf ) {
          try {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 2. try reader " << ie->second << std::endl;
            }
            reader->setupAndRead( obj, _datasourceinfo, _alloccontext, _options );
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 2. " << ie->second << " OK\n";
            }
            return true;
          } catch( std::exception & e ) {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 2. failed\n";
            }
            carto::io_error::keepExceptionPriority( e, excp, exct, excm );
          }
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 2. unsuccessfully tried " << ie->second 
                      << std::endl;
          }
          tried.insert( ie->second );
          triedf.insert( reader );
        }
      }
    }
    
    //// Pass 3 : not found or none works: try readers with no extension ///////
    if( !ext.empty() ) {
      iext = extensions.equal_range( "" );
      
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( ie->second );
          if( reader && triedf.find( reader ) == notyetf ) {
            try {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 3. try reader " << ie->second << std::endl;
              }
              reader->setupAndRead( obj, _datasourceinfo, _alloccontext, _options );
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 3. " << ie->second << " OK\n";
              }
              return true;
            } catch( std::exception & e ) {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 3. failed\n";
              }
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 4 : still not found ? well, try EVERY format this time... ////////
    iext.first = extensions.begin();
    iext.second = extensions.end();

    for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
      if( tried.find( (*ie).second ) == notyet ) {
        reader = FormatDictionary<T>::readFormat( ie->second );
        if( reader && triedf.find( reader ) == notyetf ) {
          try {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 4. try reader " << ie->second << std::endl;
            }
            reader->setupAndRead( obj, _datasourceinfo, _alloccontext, _options );
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 4. " << ie->second << " OK\n";
            }
            return true;
          } catch( std::exception & e ) {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 4. failed\n";
            }
            carto::io_error::keepExceptionPriority( e, excp, exct, excm );
          }
          tried.insert( ie->second );
          triedf.insert( reader );
        }
      }
    }

    //// End : still not succeeded, it's hopeless... ///////////////////////////
    carto::io_error::launchExcept( exct, excm, filename + " : no matching format" );
    return false;
  }

  template<class T>
  T* Reader<T>::read( carto::Object header )
  {
    if( carto::debugMessageLevel > 3 ) {
      std::cout << "READER:: <" << carto::DataTypeCode<T>::name() << ">\n";
    }
    
    if( !header.isNone() )
      _datasourceinfo->header() = header;

    if( !dataSource() )
      throw std::runtime_error( "Reader with no source of data" );
    if( !_options.get() )
      _options = carto::Object::value( carto::PropertySet() );
    
    //// Checking format ///////////////////////////////////////////////////////
    DataSourceInfoLoader  dsil; // manages the case of a not-none header
    DataSourceInfo        dsi = dsil.check( *_datasourceinfo, _options );
    if( dsi.list().empty() )
      dsil.launchException();
    if( dsi.header().get() )
      dsil.launchException();
    *_datasourceinfo = dsi;
    _alloccontext.setDataSourceInfo( _datasourceinfo );
    
    std::string format;
    if( !_options->getProperty( "format", format )  // user options first
        && !_datasourceinfo->header()->getProperty( "format", format ) ) // new style
      _datasourceinfo->header()->getProperty( "file_type", format ); // old style
    
    if( carto::debugMessageLevel > 3 ) {
      std::cout << "READER:: format: " << format << std::endl;
    }

    //// Reading data //////////////////////////////////////////////////////////
    std::string   filename  = _datasourceinfo->list()
                                             .dataSource( "default", 0 )
                                             ->url();
    set_S                         tried;
    std::set<FormatReader<T> *>   triedf;
    FormatReader<T>			          *reader;
    set_S::iterator               notyet = tried.end();
    typename std::set<FormatReader<T> *>::iterator  notyetf = triedf.end();
    T            *obj;
    int          excp = 0;
    int          exct = -1;
    std::string  excm;

    //// Pass 1 : prioroty to format hint //////////////////////////////////////
    if( !format.empty() )	{
      reader = FormatDictionary<T>::readFormat( format );
      if( reader ) {
        try {
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 1. try reader " << format << std::endl;
          }
          obj = reader->createAndRead( _datasourceinfo, _alloccontext, _options );
          if( obj ) {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 1. " << format << " OK\n";
            }
            return obj;
          }
	      } catch( std::exception & e ) {
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 1. failed\n";
          }
          carto::io_error::keepExceptionPriority( e, excp, exct, excm, 5 );
	      }
        tried.insert( format );
        triedf.insert( reader );
      }
    }

    std::string	ext = carto::FileUtil::extension( filename );

    const multi_S &  extensions = FormatDictionary<T>::readExtensions();

    pair_cit_S               iext = extensions.equal_range( ext );
    multi_S::const_iterator  ie, ee = iext.second;

    //// Pass 2 : try every matching format until one works ////////////////////
    for( ie=iext.first; ie!=ee; ++ie ) {
      if( tried.find( (*ie).second ) == notyet ) {
        reader = FormatDictionary<T>::readFormat( (*ie).second );
        if( reader && triedf.find( reader ) == notyetf ) {
          try {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 2. try reader " << ie->second << std::endl;
            }
            obj = reader->createAndRead( _datasourceinfo, _alloccontext, _options );
            if( obj ) {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 2. " << ie->second << " OK\n";
              }
              return obj;
            }
          } catch( std::exception & e ) {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 2. failed\n";
            }
            carto::io_error::keepExceptionPriority( e, excp, exct, excm );
          }
          if( carto::debugMessageLevel > 3 ) {
            std::cout << "READER:: 2. unsuccessfully tried " << ie->second 
                      << std::endl;
          }
          tried.insert( ie->second );
          triedf.insert( reader );
        }
      }
    }

    if( !ext.empty() ) {
      iext = extensions.equal_range( "" );

      //// Pass 3 : not found or none works: try readers with no extension /////
      for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
        if( tried.find( (*ie).second ) == notyet ) {
          reader = FormatDictionary<T>::readFormat( (*ie).second );
          if( reader && triedf.find( reader ) == notyetf ) {
            try {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 3. try reader " << ie->second << std::endl;
              }
              obj = reader->createAndRead( _datasourceinfo, _alloccontext, _options );
              if( obj ) {
                if( carto::debugMessageLevel > 3 ) {
                  std::cout << "READER:: 3. " << ie->second << " OK\n";
                }
                return obj;
              }
            } catch( std::exception & e ) {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 3. failed\n";
              }
              carto::io_error::keepExceptionPriority( e, excp, exct, excm );
            }
            tried.insert( ie->second );
            triedf.insert( reader );
          }
        }
      }
    }

    //// Pass 4 : still not found ? well, try EVERY format this time... ////////
    iext.first = extensions.begin();
    iext.second = extensions.end();

    for( ie=iext.first, ee=iext.second; ie!=ee; ++ie ) {
      if( tried.find( (*ie).second ) == notyet ) {
        reader = FormatDictionary<T>::readFormat( ie->second );
        if( reader && triedf.find( reader ) == notyetf ) {
          try {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 4. try reader " << ie->second << std::endl;
            }
            obj = reader->createAndRead( _datasourceinfo, _alloccontext, _options );
            if( obj ) {
              if( carto::debugMessageLevel > 3 ) {
                std::cout << "READER:: 4. " << ie->second << " OK\n";
              }
              return obj;
            }
          } catch( std::exception & e ) {
            if( carto::debugMessageLevel > 3 ) {
              std::cout << "READER:: 4. failed\n";
            }
            carto::io_error::keepExceptionPriority( e, excp, exct, excm );
          }
          tried.insert( ie->second );
          triedf.insert( reader );
        }
      }
    }

    //// End : still not succeeded, it's hopeless... ///////////////////////////
    carto::io_error::launchExcept( exct, excm, filename + " : no matching format" );
    return 0;
  }

}

#endif