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


soma::SiemensDiffusionModule::SiemensDiffusionModule()
                            : soma::DiffusionModule()
{
}


bool soma::SiemensDiffusionModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Sint32 tmpInt;

    if ( dataset->findAndGetSint32( DcmTagKey( 0x0019, 0x100c ),
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

          if ( dataset->findAndGetFloat64( DcmTagKey( 0x0019, 0x100e ),
                                           tmpDouble,
                                           d ).good() )
          {

            direction[ d ] = double( tmpDouble );

          }

        }

        direction[ 2 ] *= -1.0;

      }

      _directions.push_back( direction );

      return true;

    }

  }

  return false;

}


int32_t soma::SiemensDiffusionModule::getStep( 
                                 soma::DicomDatasetHeader& /* datasetHeader */ )
{

  return 1;

}

