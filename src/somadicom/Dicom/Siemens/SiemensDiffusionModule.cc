#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/SiemensDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/Siemens/SiemensDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::SiemensDiffusionModule::SiemensDiffusionModule()
                           : dcm::DiffusionModule()
{
}


bool dcm::SiemensDiffusionModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Sint32 tmpInt;

    if ( dcmItem->findAndGetSint32( DcmTagKey( 0x0019, 0x100c ),
                                    tmpInt ).good() )
    {

      std::vector< double > direction( 3, 0.0 );

      _bValues.push_back( double( tmpInt ) );

      if ( tmpInt )
      {

        int32_t d;
        Float64 tmpDouble;

        for ( d = 0; d < 3; d++ )
        {

          if ( dcmItem->findAndGetFloat64( DcmTagKey( 0x0019, 0x100e ),
                                           tmpDouble,
                                           d ).good() )
          {

            direction[ d ] = double( tmpDouble );

          }

        }

      }

      addDirection( direction );

      return true;

    }

  }

  return false;

}


int32_t dcm::SiemensDiffusionModule::getStep( 
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  OFString imageType;
  DcmDataset dataset;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFStringArray( DCM_ImageType, imageType ).good() )
  {

    if ( imageType.find( "MOSAIC" ) != std::string::npos )
    {

      return 1;

    }

  }

  return dcm::DiffusionModule::getStep( datasetHeader );

}
