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

#ifndef SOMAIO_IMAGE_DICOMIMAGEWRITER_D_H
#define SOMAIO_IMAGE_DICOMIMAGEWRITER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/dicomimagewriter.h>                 // class declaration
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Container/DicomProxy.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/io/writer.h>                                // to write minf
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/streamdatasource.h>      // used by writeHeader()
#include <soma-io/datasource/datasource.h>
#include <soma-io/checker/dicomformatchecker.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/object/property.h>                      // header, options
#include <cartobase/type/types.h>                           // to write header
#include <cartobase/stream/fileutil.h>                          // utilitaires
#include <cartobase/type/voxelrgb.h>
#include <cartobase/config/version.h>
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "DICOMIMAGEWRITER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T>
  void DicomImageWriter<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4, 1) );
    _voxel_size = std::vector<float>( 4, 1.0f );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
    dsi.header()->getProperty( "voxel_size", _voxel_size );
  }

  template <typename T>
  void DicomImageWriter<T>::resetParams()
  {
    _sizes = std::vector< std::vector<int> >();
  }

  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template <typename T>
  DicomImageWriter<T>::DicomImageWriter() : ImageWriter<T>()
  {
  }

  template <typename T>
  DicomImageWriter<T>::~DicomImageWriter()
  {
  }

  //==========================================================================
  //   I M A G E W R I T E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void DicomImageWriter<T>::write( const T * source, DataSourceInfo & dsi,
                                   const std::vector<int> & /* pos */,
                                   const std::vector<int> & /* size */,
                                   const std::vector<long> & /* strides */,
                                   carto::Object /* options */ )
  {    
    if( _sizes.empty() )
      updateParams( dsi );

    dcm::DataInfo dataInfo;
    std::string dataType = carto::DataTypeCode< T >::dataType();
    localMsg("dataType:" + carto::toString(dataType));
    std::string basename = dsi.list().dataSource( "dcm", 0 )->url();
    localMsg("basename:" + carto::toString(basename));
    dataInfo._pixelRepresentation = 0;

    if ( dataType == "U8" )
    {
      dataInfo._depth = 8;
      dataInfo._spp = 1;
    }
    else if ( dataType == "U16" )
    {
      dataInfo._depth = 16;
      dataInfo._spp = 1;
    }
    else if ( dataType == "S16" )
    {
      dataInfo._depth = 16;
      dataInfo._spp = 1;
      dataInfo._pixelRepresentation = 1;
    }
    else if ( dataType == "RGB" )
    {
      dataInfo._depth = 8;
      dataInfo._spp = 3;
    }
    else
    {
      localMsg("datatype_format_error for dataType:" + carto::toString(dataType));
      throw datatype_format_error( 
                                 "DICOM writer does not support this data type",
                                 basename );
    }
    
    dataInfo._invertLut = false;
    dataInfo._width = _sizes[ 0 ][ 0 ];
    dataInfo._height = _sizes[ 0 ][ 1 ];
    dataInfo._slices = _sizes[ 0 ][ 2 ];
    dataInfo._frames = _sizes[ 0 ][ 3 ];
    dataInfo._resolution.x = _voxel_size[ 0 ];
    dataInfo._resolution.y = _voxel_size[ 1 ];
    dataInfo._resolution.z = _voxel_size[ 2 ];
    dataInfo._noFlip = true;
    dataInfo._boundingBox.setUpperX( _sizes[ 0 ][ 0 ] - 1 );
    dataInfo._boundingBox.setUpperY( _sizes[ 0 ][ 1 ] - 1 );
    dataInfo._boundingBox.setUpperZ( _sizes[ 0 ][ 2 ] - 1 );
    dataInfo._boundingBox.setUpperT( _sizes[ 0 ][ 3 ] - 1 );

    localMsg("DicomProxy declaration");
    dcm::DicomProxy data( (uint8_t*)source, &dataInfo );
    
    localMsg("DicomProxy allocation");
    data.allocate(); // to set up accessors
    
    localMsg("DicomIO writting");
    dcm::DicomIO::getInstance().write( basename, data );
    
  }

  template <typename T>
  DataSourceInfo DicomImageWriter<T>::writeHeader(
    DataSourceInfo & dsi,
    const T * /* source */,
    const std::vector<int> & /* pos */,
    const std::vector<int> & /* size */,
    const std::vector<long> & /* strides */,
    carto::Object /* options */ )
  {
    bool dolist = dsi.list().typecount() == 1;

    if( dolist )
    {
      localMsg( "building DataSourceList..." );
      buildDSList( dsi.list() );
    }

    //------------------------------------------------------------------------
    localMsg( "done writing header." );
    return dsi;
  }

  //==========================================================================
  //   U T I L I T I E S
  //==========================================================================
  template <typename T>
  void DicomImageWriter<T>::buildDSList( DataSourceList & dsl ) const
  {
    DataSource* pds = dsl.dataSource().get();
    std::string basename, dcmname = pds->url();

    if( dcmname.empty() )
    {
      throw carto::file_not_found_error( "needs a filename", dcmname );
    }
    else
    {
      dsl.addDataSource( "dcm", carto::rc_ptr<DataSource>( pds ) );
    }
  }

}

#endif
