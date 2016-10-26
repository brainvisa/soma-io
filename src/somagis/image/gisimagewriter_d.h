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

#ifndef SOMAIO_IMAGE_GISIMAGEWRITER_D_H
#define SOMAIO_IMAGE_GISIMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/gisimagewriter.h>                 // class declaration
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
#include <soma-io/writer/itemwriter.h>                     // write + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
#include <cartobase/containers/nditerator.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
#define SOMAIO_BYTE_ORDER 0x41424344 //"ABCD" in ascii- > used for byteswap
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "GISIMAGEWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void GisImageWriter<T>::updateParams( DataSourceInfo & dsi )
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
    if( !dsi.header()->getProperty( "volume_dimension", _sizes[ 0 ] ) )
    {
      dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
      dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
      dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
      dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    }

    ChainDataSource::setSource( dsi.list().dataSource( "ima" ),
                                dsi.list().dataSource( "ima" )->url() );
  }

  template <typename T>
  void GisImageWriter<T>::resetParams()
  {
    _binary = true;
    _byteswap = false;
    _sizes = std::vector< std::vector<int> >();
  }

  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  GisImageWriter<T>::GisImageWriter() :
    ImageWriter<T>(),
    ChainDataSource( DataSource::none() ),
    _itemw()
  {
  }

  template <typename T>
  GisImageWriter<T>::~GisImageWriter()
  {
  }

  //==========================================================================
  //   C H A I N D A T A S O U R C E   M E T H O D S
  //==========================================================================
  template <typename T>
  DataSource* GisImageWriter<T>::clone() const
  {
    return new FileDataSource( ChainDataSource::url() );
  }

  template <typename T>
  int GisImageWriter<T>::iterateMode() const
  {
    int m = source()->iterateMode();
    // direct access is not possible on ascii streams
    if( _binary )
      m &= SequentialAccess;
    return m;
  }

  template <typename T>
  offset_t GisImageWriter<T>::size() const
  {
    return source() ? source()->size() : 0;
  }

  template <typename T>
  offset_t GisImageWriter<T>::at() const
  {
    return source()->at();
  }

  template <typename T>
  bool GisImageWriter<T>::at( offset_t pos )
  {
    return source()->at( pos );
  }

  template <typename T>
  long GisImageWriter<T>::readBlock( char * data, unsigned long maxlen )
  {
    return 0;
  }


  template <typename T>
  long GisImageWriter<T>::writeBlock( const char * data, unsigned long len )
  {
    if( !_itemw.get() )
    {
      DefaultItemWriter<T>      diw;
      _itemw.reset( diw.writer( _binary, _byteswap ) );
    }
    unsigned long nitems = len / sizeof( T );
    return _itemw->write( *source(), (const T *) data, nitems ) * sizeof( T );
  }


  template <typename T>
  int GisImageWriter<T>::getch()
  {
    return source()->getch();
  }

  template <typename T>
  int GisImageWriter<T>::putch( int ch )
  {
    return source()->putch( ch );
  }

  template <typename T>
  bool GisImageWriter<T>::ungetch( int ch )
  {
    return source()->ungetch( ch );
  }

  template <typename T>
  bool GisImageWriter<T>::allowsMemoryMapping() const
  {
    return _binary && !_byteswap;

  }

  template <typename T>
  bool GisImageWriter<T>::setpos( const std::vector<int> & pos )
  {
    offset_t off = pos[0], dimoff = 1;
    size_t i, n = pos.size();
    for( i=0; i<n - 1; ++i )
    {
      dimoff *= _sizes[0][i];
      off += dimoff * pos[ i + 1 ];
    }
    return source()->at( ( (offset_t) sizeof(T) ) * off );
  }

  //==========================================================================
  //   I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void GisImageWriter<T>::write( const T * source, DataSourceInfo & dsi,
                                 const std::vector<int> & pos,
                                 const std::vector<int> & size,
                                 const std::vector<long> & strides,
                                 carto::Object options )
  {
    if( _sizes.empty() )
      updateParams( dsi );

    size_t ndim = size.size();

    // region line size
    offset_t  len = ((offset_t) size[0]) * sizeof( T );

    bool mustclose = !isOpen();

    if( options->hasProperty( "partial_writing" )
        && !open( DataSource::ReadWrite ) )
      throw carto::open_error( "data source not available", url() );
    else if( !open( DataSource::Write ) )
      throw carto::open_error( "data source not available", url() );

    try
    {
      if( !source ) // unallocated data
      {
        std::vector<int> szend = _sizes[ 0 ];
        int dim;
        for( dim=0; dim<ndim; ++dim )
          --szend[dim];
        setpos( szend );
        T value = 0;
        if( writeBlock( reinterpret_cast<const char *>( &value ), sizeof(T) )
            != (long) sizeof(T) )
          throw carto::eof_error( url() );
      }
      else
      {
        std::vector<int> dpos( ndim, 0 );
        int dim;
        dpos[0] = pos[0];
        const_line_NDIterator<T> it( source, size, strides );

        for( ; !it.ended(); ++it )
        {
          for( dim=1; dim<ndim; ++dim )
            dpos[dim] = it.position()[dim] + pos[dim];
          // we move in the file
          setpos( dpos );
          // we move in the buffer
          // FIXME: stride[0] not taken into account for now
          const char * target = (char *) &*it;
          if( writeBlock( target, len ) != (long) len )
              throw carto::eof_error( url() );
        }
      }
    }
    catch( std::exception & e )
    {
      if( mustclose )
      {
        close();
        carto::rc_ptr<DataSource> hds = dsi.list().dataSource( "dim" );
        if( hds )
          hds->close();
      }
      throw;
    }
    if( mustclose )
    {
      close();
      carto::rc_ptr<DataSource> hds = dsi.list().dataSource( "dim" );
      if( hds )
        hds->close();
    }
  }

  template <typename T>
  DataSourceInfo GisImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T *,
    const std::vector<int> &,
    const std::vector<int> &,
    const std::vector<long> &,
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
      std::vector<int> vdim;
      if( dimdsi.header()->getProperty( "volume_dimension", vdim ) )
      {
        dsi.header()->setProperty( "volume_dimension", vdim );
        dsi.header()->setProperty( "sizeX", vdim.size() > 0 ? vdim[0] : 1 );
        dsi.header()->setProperty( "sizeY", vdim.size() > 1 ? vdim[1] : 1 );
        dsi.header()->setProperty( "sizeZ", vdim.size() > 2 ? vdim[2] : 1 );
        dsi.header()->setProperty( "sizeT", vdim.size() > 3 ? vdim[3] : 1 );
      }
      else
      {
        dsi.header()->setProperty( "sizeX", dimdsi.header()->getProperty( "sizeX" ) );
        dsi.header()->setProperty( "sizeY", dimdsi.header()->getProperty( "sizeY" ) );
        dsi.header()->setProperty( "sizeZ", dimdsi.header()->getProperty( "sizeZ" ) );
        dsi.header()->setProperty( "sizeT", dimdsi.header()->getProperty( "sizeT" ) );
      }
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
    if( !dsi.header()->getProperty( "volume_dimension", dim ) )
    {
      dsi.header()->getProperty( "sizeX", dim[0] );
      dsi.header()->getProperty( "sizeY", dim[1] );
      dsi.header()->getProperty( "sizeZ", dim[2] );
      dsi.header()->getProperty( "sizeT", dim[3] );
    }
    std::vector<float> vs( dim.size(), 1. );
    // reading voxel size
    try
    {
      carto::Object vso = dsi.header()->getProperty( "voxel_size" );
      if( vso.get() )
      {
        int i, nvs = std::min( vso->size(), dim.size() );
        for( i=0; i<nvs; ++i )
          vs[i] = (float) vso->getArrayItem(i)->getScalar();
      }
    }
    catch( ... )
    {
    }
    // header :: volume dimensions
    size_t i, ndim = dim.size();
    for( i=0; i<ndim - 1; ++i )
      *ds << dim[i] << " ";
    *ds << dim[ ndim - 1 ] << "\n";
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
    carto::Object minf = dsi.header();
    if( minf->hasProperty( "file_type" ) )
      minf->removeProperty( "file_type" );
    minf->setProperty( "format", std::string( "GIS" ) );
    minf->setProperty( "data_type", carto::DataTypeCode<T>::dataType() );
    minf->setProperty( "object_type", std::string( "Volume" ) );
    std::vector<int> dims( 4, 0 );
    if( !minf->getProperty( "volume_dimension", dims ) )
    {
      minf->getProperty( "sizeX", dims[ 0 ] );
      minf->getProperty( "sizeY", dims[ 1 ] );
      minf->getProperty( "sizeZ", dims[ 2 ] );
      minf->getProperty( "sizeT", dims[ 3 ] );
      minf->setProperty( "volume_dimension", dims );
    }
    minf->setProperty( "voxel_size",
                       minf->getProperty( "voxel_size" ) );

    Writer<carto::GenericObject> minfw( dsi.list().dataSource( "minf" ) );
    minfw.write( *minf );
//     //--- partial-io case ----------------------------------------------------
//     if( options->hasProperty( "unallocated" ) ) {
//       localMsg( "building file for partial writing..." );
//       if( _sizes.empty() )
//         updateParams( dsi );
//       ChainDataSource::setSource( dsi.list().dataSource( "ima" ),
//                                   dsi.list().dataSource( "ima" )->url() );
//
//       bool mustclose = !isOpen();
//       if( !open( DataSource::Write ) )
//         throw carto::open_error( "data source not available", url() );
//
//       try {
//         T value[1] = {0};
//         setpos( dim[0]-1, dim[1]-1, dim[2]-1, dim[3]-1);
//         localMsg( "book " + carto::toString((long)at()+sizeof(T)) );
//         if( writeBlock( (char * ) value, sizeof(T) ) != (long) sizeof(T) )
//           throw carto::eof_error( url() );
//       }
//       catch( ... ) {
//         if( mustclose ) {
//           close();
//         }
//         throw;
//       }
//       if( mustclose ) {
//         close();
//       }
//     }

    //------------------------------------------------------------------------
    localMsg( "done writing header." );
    return dsi;
  }

  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T>
  void GisImageWriter<T>::buildDSList( DataSourceList & dsl,
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
