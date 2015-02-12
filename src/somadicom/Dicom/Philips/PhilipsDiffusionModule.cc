#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Philips/PhilipsDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/Philips/PhilipsDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::PhilipsDiffusionModule::PhilipsDiffusionModule()
                            : soma::DiffusionModule()
{
}


bool soma::PhilipsDiffusionModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float32 tmpFloat;
    
    if ( dataset->findAndGetFloat32( DcmTagKey( 0x2001, 0x1003 ), 
                                     tmpFloat ).good() )
    {

      std::vector< double > direction( 3, 0.0 );

      _bValues.push_back( double( tmpFloat ) );

      if ( tmpFloat > 0.0f )
      {

        if ( dataset->findAndGetFloat32( DcmTagKey( 0x2005, 0x10b0 ),
                                         tmpFloat ).good() )
        {

          direction[ 0 ] = double( tmpFloat );

        }

        if ( dataset->findAndGetFloat32( DcmTagKey( 0x2005, 0x10b1 ),
                                         tmpFloat ).good() )
        {

          direction[ 1 ] = double( tmpFloat );

        }

        if ( dataset->findAndGetFloat32( DcmTagKey( 0x2005, 0x10b2 ),
                                         tmpFloat ).good() )
        {

          direction[ 2 ] = -1.0 * double( tmpFloat );

        }

      }

      _directions.push_back( direction );

      return true;

    }

  }

  return false;

}


int32_t soma::PhilipsDiffusionModule::getStep( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  Sint32 tmpInt;
  DcmDataset dataset;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetSint32( DcmTagKey( 0x2001, 0x1018 ), 
                                 tmpInt ).good() )
  {

    return int32_t( tmpInt );

  }

  return soma::DiffusionModule::getStep( datasetHeader );

}

