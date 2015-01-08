#include <soma-io/Dicom/GE/GEMRReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::GEMRReader::GEMRReader()
                : soma::MRImageStorageReader(),
                  soma::Singleton< soma::GEMRReader >()
{
}


soma::GEMRReader::~GEMRReader()
{
}


std::string soma::GEMRReader::getManufacturerName()
{

  return "GE MEDICAL SYSTEMS";

}


bool soma::GEMRReader::getHeader( soma::HeaderProxy& proxy,
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
    Sint32 tmpInt;

    datasetHeader.get( dataset );

    if ( dataset.findAndGetSint32( DcmTagKey( 0x0043, 0x1039 ), 
                                   tmpInt, 
                                   0 ).good() )
    {

      int32_t i, n = datasetHeader.size();
      int32_t step = info.m_slices;
      std::vector< double > bValues;
      std::vector< std::vector< double > > directions;

      for ( i = 0; i < n; i += step )
      {

        datasetHeader.get( dataset, i );

        if ( dataset.findAndGetSint32( DcmTagKey( 0x0043, 0x1039 ),
                                       tmpInt,
                                       0 ).good() )
        {

          bValues.push_back( double( tmpInt ) );

        }

        std::vector< double > direction( 3, 0.0 );

        if ( tmpInt > 0 )
        {

          Float64 tmpFloat;
          OFString tmpString;
          std::string encodingDirection = "COL";

          if ( dataset.findAndGetOFString( DCM_InPlanePhaseEncodingDirection,
                                           tmpString ).good() )
          {

            encodingDirection = tmpString.c_str();

          }

          if ( encodingDirection == "COL" )
          {

            if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                            tmpFloat ).good() )
            {

              direction[ 0 ] = double( tmpFloat );

            }

            if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                            tmpFloat ).good() )
            {

              direction[ 1 ] = double( tmpFloat );

            }

          }
          else if (  encodingDirection == "ROW" )
          {

            if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x10bb ),
                                            tmpFloat ).good() )
            {

              direction[ 1 ] = double( tmpFloat );

            }

            if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x10bc ),
                                            tmpFloat ).good() )
            {

              direction[ 0 ] = double( tmpFloat );

            }

          }

          if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x10bd ),
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


RegisterDicomReaderFunction( GEMRReader );

