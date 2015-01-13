#include <soma-io/Dicom/Philips/PhilipsMRReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsMRReader::PhilipsMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::PhilipsMRReader >()
{
}


soma::PhilipsMRReader::~PhilipsMRReader()
{
}


std::string soma::PhilipsMRReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsMRReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    DcmDataset dataset;
    Float32 tmpFloat;

    datasetHeader.get( dataset );

    if ( dataset.findAndGetFloat32( DcmTagKey( 0x2001, 0x1003 ), 
                                    tmpFloat ).good() )
    {

      int32_t i, n = datasetHeader.size();
      int32_t dummyX, dummyY, step;
      std::vector< double > bValues;
      std::vector< std::vector< double > > directions;

      info.m_patientOrientation.getOnDiskSize( dummyX, dummyY, step );

      for ( i = 0; i < n; i += step )
      {

        datasetHeader.get( dataset, i );

        if ( dataset.findAndGetFloat32( DcmTagKey( 0x2001, 0x1003 ),
                                        tmpFloat ).good() )
        {

          bValues.push_back( double( tmpFloat ) );

        }

        std::vector< double > direction( 3, 0.0 );

        if ( tmpFloat > 0.0f )
        {

          if ( dataset.findAndGetFloat32( DcmTagKey( 0x2005, 0x10b0 ),
                                          tmpFloat ).good() )
          {

            direction[ 0 ] = double( tmpFloat );

          }

          if ( dataset.findAndGetFloat32( DcmTagKey( 0x2005, 0x10b1 ),
                                          tmpFloat ).good() )
          {

            direction[ 1 ] = double( tmpFloat );

          }

          if ( dataset.findAndGetFloat32( DcmTagKey( 0x2005, 0x10b2 ),
                                          tmpFloat ).good() )
          {

            direction[ 2 ] = double( tmpFloat );

          }

        }

        directions.push_back( direction );

      }

      if ( bValues.size() )
      {

        proxy.addAttribute( "b_values", bValues );

      }

      if ( directions.size() )
      {

        proxy.addAttribute( "diffusion_directions", directions );

      }

    }

  }

  return true;

}


RegisterDicomReaderFunction( PhilipsMRReader );

