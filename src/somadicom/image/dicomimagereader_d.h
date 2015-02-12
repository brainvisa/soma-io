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

#ifndef SOMAIO_IMAGE_DICOMIMAGEREADER_D_H
#define SOMAIO_IMAGE_DICOMIMAGEREADER_D_H
//--- plugin -----------------------------------------------------------------
#include <soma-io/image/dicomimagereader.h>                 // class declaration
#include <soma-io/Dicom/DicomIO.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DataInfoCache.h>
#include <soma-io/Container/BoundingBox.h>
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasourceinfo/datasourceinfo.h>      // function's argument
#include <soma-io/datasource/filedatasource.h>              // used by clone()
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
#include <string>
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#define localMsg( message ) cartoCondMsg( 4, message, "DICOMIMAGEREADER" )
// localMsg must be undef at end of file
//----------------------------------------------------------------------------

namespace soma {
  
  //==========================================================================
  //   U S E F U L
  //==========================================================================
  template <typename T> 
  void DicomImageReader<T>::updateParams( DataSourceInfo & dsi )
  {
    _sizes = std::vector< std::vector<int> >( 1, std::vector<int>(4) );
    dsi.header()->getProperty( "sizeX", _sizes[ 0 ][ 0 ] );
    dsi.header()->getProperty( "sizeY", _sizes[ 0 ][ 1 ] );
    dsi.header()->getProperty( "sizeZ", _sizes[ 0 ][ 2 ] );
    dsi.header()->getProperty( "sizeT", _sizes[ 0 ][ 3 ] );
  }
  
  template <typename T> 
  void DicomImageReader<T>::resetParams()
  {
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
  DicomImageReader<T>::DicomImageReader() : ImageReader<T>()
  {
  }
  
  template <typename T>
  DicomImageReader<T>::~DicomImageReader()
  {
  }
  
  //==========================================================================
  //   I M A G E R E A D E R   M E T H O D S
  //==========================================================================
  template <typename T>
  void DicomImageReader<T>::read( T * dest, DataSourceInfo & dsi,
                                  std::vector<int> & pos,
                                  std::vector<int> & size,
                                  std::vector<long> & /* stride */,
                                  carto::Object      /* options */ )
  {

    if ( dest )
    {

      if( _sizes.empty() )
        updateParams( dsi );

      // region position
      int ox = pos[ 0 ];
      int oy = pos[ 1 ];
      int oz = pos[ 2 ];
      int ot = pos[ 3 ];

      // region opposite position
      int lx = ox + size[ 0 ] - 1;
      int ly = oy + size[ 1 ] - 1;
      int lz = oz + size[ 2 ] - 1;
      int lt = ot + size[ 3 ] - 1;

      soma::DataInfo& info = soma::DataInfoCache::getInstance().getDataInfo();
      soma::DicomDatasetHeader datasetHeader( info );
      std::vector< std::string >& fileList = datasetHeader.getFileList();
      int i, count = dsi.list().size( "dicom" );

      for ( i = 0; i < count; i++ )
      {

        fileList.push_back( dsi.list().dataSource( "dicom", i )->url() );

      }
    
      if ( !fileList.empty() )
      {

        BoundingBox< int32_t > bbox( ox, lx, oy, ly, oz, lz, ot, lt );

        info._boundingBox = info._patientOrientation.getInverseBoundingBox( 
                                                                         bbox );

        DicomProxy data( (uint8_t*)dest, &info );
        DicomIO::getInstance().read( datasetHeader, data );
        soma::DataInfoCache::getInstance().clear();

      }
    }
  }


  template <typename T>
  ImageReader<T>* DicomImageReader<T>::cloneReader() const
  {
    return new DicomImageReader;
  }

}

#undef localMsg
#endif
