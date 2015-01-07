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

#ifndef SOMAIO_IMAGE_GISIMAGEREADER_D_H
#define SOMAIO_IMAGE_GISIMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/gisimagereader.h>                 // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/reader/itemreader.h>                      // read + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "GISIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {
  
  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T> 
  void GisImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    try {
      _binary = !(bool) dsi.header()->getProperty( "ascii" )->getScalar();
    } catch( ... ) {
      _binary = true;
    }
    
    try {
      _byteswap = (bool) dsi.header()->getProperty( "byte_swapping" )
                                     ->getScalar();
    } catch( ... ) {
      _byteswap = false;
    }
    
    DefaultItemReader<T>  dir;
    _itemr.reset( dir.reader( _binary, _byteswap ) );
    
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    
    ChainDataSource::setSource( dsi.list().dataSource( "ima", 0 ), 
                                dsi.list().dataSource( "ima", 0)->url() );
  }
  
  template <typename T> 
  void GisImageReader<T>::resetParams()
  {
    _binary = true;
    _byteswap = false;
    _sizes = std::vector< std::vector<int> >();
  }
  
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  /* base constructors usage :
   * - FileDataSource( rc_ptr<DataSource> ds, const string & url = string() );
   * - /!\ no copy constructor
   * - ImageReader( DataSourceInfo & dsi = 0, bool threadsafe = false );
   * - ImageReader( const ImageReader<T> & );
   */
  
  template <typename T>
  GisImageReader<T>::GisImageReader() :
    ImageReader<T>(),
    ChainDataSource( DataSource::none() ),
    _itemr()
  {
  }
  
  template <typename T>
  GisImageReader<T>::~GisImageReader()
  {
  }
  
  //==========================================================================
  //   C H A I N D A T A S O U R C E   M E T H O D S
  //==========================================================================
  template <typename T> 
  DataSource* GisImageReader<T>::clone() const
  {
    return new FileDataSource( ChainDataSource::url() );
  }
  
  template <typename T> 
  int GisImageReader<T>::iterateMode() const
  {
    int m = source()->iterateMode();
    // direct access is not possible on ascii streams
    if( _binary )
      m &= SequentialAccess;
    return m;
  }
  
  template <typename T> 
  offset_t GisImageReader<T>::size() const
  {
    return source() ? source()->size() : 0;
  }
  
  template <typename T> 
  offset_t GisImageReader<T>::at() const
  {
    return source()->at();
  }
  
  template <typename T> 
  bool GisImageReader<T>::at( offset_t pos )
  {
    return source()->at( pos );
  }
  
  template <typename T> 
  long GisImageReader<T>::readBlock( char * data, unsigned long maxlen )
  {
    if( !_itemr.get() )
    {
      DefaultItemReader<T>      dir;
      _itemr.reset( dir.reader( _binary, _byteswap ) );
    }
    unsigned long nitems = maxlen / sizeof( T );
    return _itemr->read( *source(), (T *) data, nitems ) * sizeof( T );
  }
  
  template <typename T> 
  long GisImageReader<T>::writeBlock( const char * data, unsigned long len )
  {
    return 0;
  }
  
  template <typename T> 
  int GisImageReader<T>::getch()
  {
    return source()->getch();
  }
  
  template <typename T> 
  int GisImageReader<T>::putch( int ch )
  {
    return source()->putch( ch );
  }
  
  template <typename T> 
  bool GisImageReader<T>::ungetch( int ch )
  {
    return source()->ungetch( ch );
  }
  
  template <typename T> 
  bool GisImageReader<T>::allowsMemoryMapping() const
  {
    return _binary && !_byteswap;
    
  }
  
  template <typename T> 
  bool GisImageReader<T>::setpos( int x, int y, int z, int t )
  {
    if( !_binary )
      return false; // cannot move in file in ascii mode
    offset_t  lin = (offset_t) _sizes[ 0 ][ 0 ];
    offset_t  sli = lin * _sizes[ 0 ][ 1 ];
    offset_t  vol = sli * _sizes[ 0 ][ 2 ];
    return source()->at( ( (offset_t) sizeof(T) ) * 
                         ( x + y * lin + z * sli + t * vol ) );
  }
  
  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void GisImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                std::vector<int> & pos,
                                std::vector<int> & size,
                                std::vector<long> & /* stride */,
                                carto::Object      /* options */ )
  {
    if( _sizes.empty() )
      updateParams( dsi );
    
    // dest is supposed to be allocated
    
    // total volume size
    int  sx = _sizes[ 0 ][ 0 ];
    int  sy = _sizes[ 0 ][ 1 ];
    int  sz = _sizes[ 0 ][ 2 ];
    int  st = _sizes[ 0 ][ 3 ];
    // region size
    int  vx = size[ 0 ];
    int  vy = size[ 1 ];
    int  vz = size[ 2 ];
    int  vt = size[ 3 ];
    // region position
    int  ox = pos[ 0 ];
    int  oy = pos[ 1 ];
    int  oz = pos[ 2 ];
    int  ot = pos[ 3 ];
    int  y, z, t;
    // region line size
    offset_t  len = vx * sizeof( T );
    offset_t offset;
    long readout;

    if( !open( DataSource::Read ) )
      throw carto::open_error( "data source not available", url() );
    for( t=0; t<vt; ++t )
      for( z=0; z<vz; ++z )
        for( y=0; y<vy; ++y ) {
          // we move in the file
          setpos(ox,y+oy,z+oz,t+ot);
          // we move in the buffer
          offset = ((offset_t)t) * vz + z;
          offset = offset * vy + y;
          offset = offset * len;
          char * target = ((char *)dest) + offset;
          if( ( readout = readBlock( target, len ) ) != (long) len ) {
            localMsg( "readBlock( failed at ( " +
                      carto::toString( y ) + ", " +
                      carto::toString( z ) + ", " +
                      carto::toString( t ) + " ) : " +
                      carto::toString( readout / sizeof(T) ) + " != " +
                      carto::toString( (long) len / sizeof( T ) ) );
            throw carto::eof_error( url() );
          }
        }
  }


  template <typename T>
  ImageReader<T>* GisImageReader<T>::cloneReader() const
  {
    return new GisImageReader;
  }

}

#undef localMsg
#endif
