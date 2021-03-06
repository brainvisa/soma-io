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

#ifndef SOMAIO_IMAGE_TIFFIMAGEWRITER_D_H
#define SOMAIO_IMAGE_TIFFIMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/tiffimagewriter.h>                 // class declaration
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/io/writer.h>                                // to write minf
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasourceinfo/datasourceinfoloader.h>    // partial writing
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/streamdatasource.h>      // used by writeHeader()
#include <soma-io/datasource/datasource.h>
#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/utilities/minfutil.h>               // used by writeHeader()
#include <soma-io/writer/itemwriter.h>                     // write + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii- > used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "TIFFIMAGEWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void TiffImageWriter<T>::updateParams( DataSourceInfo & dsi )
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

    DefaultItemWriter<T>  diw;
    _itemw.reset( diw.writer( _binary, _byteswap ) );

    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );

    ChainDataSource::setSource( dsi.list().dataSource( "ima" ),
                                dsi.list().dataSource( "ima" )->url() );
  }

  template <typename T>
  void TiffImageWriter<T>::resetParams()
  {
    _binary = true;
    _byteswap = false;
    _sizes = std::vector< std::vector<int> >();
  }

  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  TiffImageWriter<T>::TiffImageWriter() :
    ImageWriter<T>(),
    ChainDataSource( DataSource::none() ),
    _itemw()
  {
  }

  template <typename T>
  TiffImageWriter<T>::~TiffImageWriter()
  {
  }

  //==========================================================================
  //   C H A I N D A T A S O U R C E   M E T H O D S
  //==========================================================================
  template <typename T>
  DataSource* TiffImageWriter<T>::clone() const
  {
    return new FileDataSource( ChainDataSource::url() );
  }

  template <typename T>
  int TiffImageWriter<T>::iterateMode() const
  {
    int m = source()->iterateMode();
    // direct access is not possible on ascii streams
    if( _binary )
      m &= SequentialAccess;
    return m;
  }

  template <typename T>
  offset_t TiffImageWriter<T>::size() const
  {
    return source() ? source()->size() : 0;
  }

  template <typename T>
  offset_t TiffImageWriter<T>::at() const
  {
    return source()->at();
  }

  template <typename T>
  bool TiffImageWriter<T>::at( offset_t pos )
  {
    return source()->at( pos );
  }

  template <typename T>
  long TiffImageWriter<T>::readBlock( char * /* data */,
                                      unsigned long /* maxlen */ )
  {
    return 0;
  }


  template <typename T>
  long TiffImageWriter<T>::writeBlock( const char * data, unsigned long len )
  {
    return writeStridedBlock( data, len, 1 );
  }


  template <typename T>
  long TiffImageWriter<T>::writeStridedBlock( const char * data,
                                              unsigned long len, long stride )
  {
    if( !_itemw.get() )
    {
      DefaultItemWriter<T>      diw;
      _itemw.reset( diw.writer( _binary, _byteswap ) );
    }
    unsigned long nitems = len / sizeof( T );
    if( stride == 1 )
      // optimal memory orientation
      return _itemw->write( *source(),
                            (const T *) data, nitems ) * sizeof( T );
    else
    {
      unsigned long chunk_size = 100000, chunk;
      long written = 0;
      std::vector<T> buffer( chunk_size );
      const T* p = reinterpret_cast<const T*>( data );
      while( nitems != 0 )
      {
        chunk = std::min( chunk_size, nitems );
        for( long i=0; i<chunk; ++i, p+=stride )
          buffer[i] = *p;
        written += _itemw->write( *source(), &buffer[0], chunk ) * sizeof( T );
        nitems -= chunk;
      }
      return written;
    }
  }


  template <typename T>
  int TiffImageWriter<T>::getch()
  {
    return source()->getch();
  }

  template <typename T>
  int TiffImageWriter<T>::putch( int ch )
  {
    return source()->putch( ch );
  }

  template <typename T>
  bool TiffImageWriter<T>::ungetch( int ch )
  {
    return source()->ungetch( ch );
  }

  template <typename T>
  bool TiffImageWriter<T>::allowsMemoryMapping() const
  {
    return _binary && !_byteswap;

  }

  template <typename T>
  bool TiffImageWriter<T>::setpos( int x, int y, int z, int t )
  {
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
  void TiffImageWriter<T>::write( const T * source, DataSourceInfo & dsi,
                                  const std::vector<int> & pos,
                                  const std::vector<int> & size,
                                  const std::vector<long> & strides,
                                  carto::Object options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    // total volume size
    //int  sx = _sizes[ 0 ][ 0 ];
    //int  sy = _sizes[ 0 ][ 1 ];
    //int  sz = _sizes[ 0 ][ 2 ];
    //int  st = _sizes[ 0 ][ 3 ];

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
    offset_t  len = ((offset_t)vx) * sizeof( T );

    if( options->hasProperty( "partial_writing" ) && !open( DataSource::ReadWrite ) )
      throw carto::open_error( "data source not available", url() );
    else if( !open( DataSource::Write ) )
      throw carto::open_error( "data source not available", url() );

    std::vector<T> buf( vx, T(0) );
    const char *buf0 = reinterpret_cast<const char *>( &buf[0] );

    for( t=0; t<vt; ++t )
      for( z=0; z<vz; ++z )
        for( y=0; y<vy; ++y ) {
          // we move in the file
          //at( ( sx * ( sy * ( sz * ( t + ot ) + z + oz )
          //                  + y + oy ) + ox ) * sizeof(T) );
          setpos(ox,y+oy,z+oz,t+ot);
          // we move in the buffer
          // FIXME: stride[0] not taken into account for now
          const char *target;
          if( source )
            target = reinterpret_cast<const char *>( source + strides[3] * t
              + strides[2] * z + strides[1] * y );
          else
            target = buf0;
          if( writeStridedBlock( target, len, strides[0] ) != (long) len )
            throw carto::eof_error( url() );
        }
  }

  template <typename T>
  DataSourceInfo TiffImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T * /* source */,
    const std::vector<int> & /* pos */,
    const std::vector<int> & /* size */,
    const std::vector<long> & /* strides */,
    carto::Object options )
  {
    //--- build datasourcelist -----------------------------------------------
    bool dolist = dsi.list().typecount() == 1 ;
    if( dolist ) {
      localMsg( "building DataSourceList..." );
      buildDSList( dsi.list(), options );
    }
    //--- set header ---------------------------------------------------------
    localMsg( "setting Header..." );
    if( options->hasProperty( "partial_writing" ) ) {
      DataSourceInfoLoader  dsil;
      DataSourceInfo dimdsi( dsi.list().dataSource( "dim" ) );
      dimdsi = dsil.check( dimdsi );
      dsi.header()->setProperty( "byte_swapping", dimdsi.header()->getProperty( "byte_swapping" ) );
      dsi.header()->setProperty( "ascii", dimdsi.header()->getProperty( "ascii" ) );
      dsi.header()->setProperty( "sizeX", dimdsi.header()->getProperty( "sizeX" ) );
      dsi.header()->setProperty( "sizeY", dimdsi.header()->getProperty( "sizeY" ) );
      dsi.header()->setProperty( "sizeZ", dimdsi.header()->getProperty( "sizeZ" ) );
      dsi.header()->setProperty( "sizeT", dimdsi.header()->getProperty( "sizeT" ) );
      return dsi;
    } else {
      if( options->hasProperty( "byte_swapping" ) )
        dsi.header()->setProperty( "byte_swapping", options->getProperty( "byte_swapping" ) );
      else
        dsi.header()->setProperty( "byte_swapping", false );
      if( options->hasProperty( "ascii" ) )
        dsi.header()->setProperty( "ascii", options->getProperty( "ascii" ) );
      else
        dsi.header()->setProperty( "ascii", false );
    }
    //--- write header -------------------------------------------------------
    localMsg( "writing Header..." );
    ChainDataSource::setSource( dsi.list().dataSource( "dim" ),
                                dsi.list().dataSource( "dim" )->url() );
    DataSource* ds;
    ds = dsi.list().dataSource( "dim" ).get();
    if( !open( DataSource::Write ) )
      throw carto::open_error( "data source not available", url() );
    // reading volume size
    std::vector<int> dim( 4, 0 );
    dsi.header()->getProperty( "sizeX", dim[0] );
    dsi.header()->getProperty( "sizeY", dim[1] );
    dsi.header()->getProperty( "sizeZ", dim[2] );
    dsi.header()->getProperty( "sizeT", dim[3] );
    std::vector<float> vs( 4, 0 );
    // reading voxel size
    vs[0] = dsi.header()->getProperty( "voxel_size")
                        ->getArrayItem(0)->getScalar();
    vs[1] = dsi.header()->getProperty( "voxel_size")
                        ->getArrayItem(1)->getScalar();
    vs[2] = dsi.header()->getProperty( "voxel_size")
                        ->getArrayItem(2)->getScalar();
    vs[3] = dsi.header()->getProperty( "voxel_size")
                        ->getArrayItem(3)->getScalar();
    // header :: volume dimensions
    *ds << dim[0] << " " << dim[1] << " "
        << dim[2] << " " << dim[3] << "\n";
    // header :: data type
    *ds << "-type " << carto::DataTypeCode<T>::dataType() << "\n";
    // header :: voxel size
    *ds << "-dx " << vs[0] << " -dy " << vs[1]
        << " -dz " << vs[2] << " -dt " << vs[3] << "\n";
    // header :: byte ordering
    *ds << "-bo ";
    uint magicNumber = SOMAIO_BYTE_ORDER;
    try {
      if( options->getProperty( "byte_swapping" )->getScalar() ) {
        uint hhbyte = ( magicNumber & 0xff000000 ) >> 24;
        uint hbyte = ( magicNumber & 0x00ff0000 ) >> 16;
        uint lbyte = ( magicNumber & 0x0000ff00 ) >> 8;
        uint llbyte = ( magicNumber & 0x000000ff);
        magicNumber = llbyte << 24 | lbyte << 16 | hbyte << 8 | hhbyte;
      }
    } catch ( ... ) {}
    ds->writeBlock( (char *) &magicNumber , sizeof(uint) );
    *ds << "\n";
    //header :: opening mode
    *ds << "-om "
        << ( dsi.header()->getProperty( "ascii" )->getScalar() ? "ascii" : "binar" )
        << "\n";
    close();
    //--- write minf ---------------------------------------------------------
    localMsg( "writing Minf..." );
    carto::Object minf = carto::Object::value( carto::PropertySet() );
    minf->setProperty( "file_type", std::string( "Tiff" ) );
    minf->setProperty( "data_type", carto::DataTypeCode<T>::dataType() );
    minf->setProperty( "object_type", std::string( "Volume" ) );
    std::vector<int> dims( 4, 0 );
    if( !dsi.header()->getProperty( "volume_dimension", dims ) )
    {
      dsi.header()->getProperty( "sizeX", dims[ 0 ] );
      dsi.header()->getProperty( "sizeY", dims[ 1 ] );
      dsi.header()->getProperty( "sizeZ", dims[ 2 ] );
      dsi.header()->getProperty( "sizeT", dims[ 3 ] );
    }
    minf->setProperty( "volume_dimension", dims );
    minf->setProperty( "voxel_size",
                       dsi.header()->getProperty( "voxel_size" ) );


    // Filter minf to remove irrelevant properties
    // and update some property (uuid) from existing minf
    soma::MinfUtil::filter(minf, options);
    soma::MinfUtil::updateFromSource(
        dsi.list().dataSource( "minf" ).get(),
        minf,
        options
    );
    
    Writer<carto::GenericObject> minfw( dsi.list().dataSource( "minf" ) );
    minfw.write( *minf );
//     //--- partial-io case ----------------------------------------------------
//     if( options->hasProperty( "unallocated" ) ) {
//       localMsg( "building file for partial writing..." );
//       if( _sizes.empty() )
//         updateParams( dsi );
//       ChainDataSource::setSource( dsi.list().dataSource( "ima" ),
//                                   dsi.list().dataSource( "ima" )->url() );
//       if( !open( DataSource::Write ) )
//         throw carto::open_error( "data source not available", url() );
//       T value[1] = {0};
//       //at( (dim[0]*dim[1]*dim[2]*dim[3]-1)*sizeof(T) );
//       setpos( dim[0]-2, dim[1]-1, dim[2]-1, dim[3]-1);
//       if( writeBlock( (char * ) value, sizeof(T) ) != (long) sizeof(T) )
//             throw carto::eof_error( url() );
//     }

    //------------------------------------------------------------------------
    localMsg( "done writing header." );
    return dsi;
  }

  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T>
  void TiffImageWriter<T>::buildDSList( DataSourceList & dsl,
                                       carto::Object /*options*/ ) const
  {
    DataSource* pds = dsl.dataSource().get();
    std::string dimname, imaname, minfname;

    dimname = imaname = minfname = pds->url();
    if( dimname.empty() ) {
      throw carto::file_not_found_error( "needs a filename", dimname );
    } else {
      std::string ext = carto::FileUtil::extension( dimname );
      std::string basename = carto::FileUtil::removeExtension( dimname );

      if( ext == "ima" ) {
        dimname = basename + ".dim";
      } else if( ext == "dim" ) {
        imaname = basename + ".ima";
      } else {
        ext.clear();
      }

      if( ext.empty() ) {
        basename = dimname;
        dimname += ".dim";
        imaname += ".ima";
      }
      minfname = imaname + ".minf";

      if( dimname == pds->url() ) {
        // if dimname is original url
        dsl.addDataSource( "dim", carto::rc_ptr<DataSource>( pds ) );
      } else {
        dsl.addDataSource( "dim", carto::rc_ptr<DataSource>
                                  ( new FileDataSource( dimname ) ) );
      }
      if( imaname == pds->url() ) {
        // if imaname is original url
        dsl.addDataSource( "ima", carto::rc_ptr<DataSource>( pds ) );
      } else {
        dsl.addDataSource( "ima", carto::rc_ptr<DataSource>
                                  ( new FileDataSource( imaname ) ) );
      }
    }

    dsl.addDataSource( "minf", carto::rc_ptr<DataSource>
                               ( new FileDataSource( minfname ) ) );

  }
}

#undef localMsg
#endif
