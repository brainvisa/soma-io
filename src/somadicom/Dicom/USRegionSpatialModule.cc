#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USRegionSpatialModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/USRegionSpatialModule.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <cmath>


soma::USRegionSpatialModule::USRegionSpatialModule()
                           : soma::DicomModule(),
                             _physicalDeltaX( 1.0 ),
                             _physicalDeltaY( 1.0 )
{
}


bool soma::USRegionSpatialModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    DcmSequenceOfItems* seq = 0;

    if ( dataset->findAndGetSequence( DCM_SequenceOfUltrasoundRegions, 
                                      seq ).good() )
    {

      bool status = false;
      Uint16 type;
      Float64 tmpDouble;
      uint32_t i, n = seq->card();

      for ( i = 0; !status && ( i < n ); i++ )
      {

        DcmItem* item = seq->getItem( i );

        if ( item->findAndGetUint16( DCM_RegionSpatialFormat, type ).good() )
        {

          if ( type == 1 )
          {

            if ( item->findAndGetFloat64( DCM_PhysicalDeltaX, 
                                          tmpDouble ).good() )
            {

              _physicalDeltaX = 10.0 * std::fabs( tmpDouble );

            }
	    
            if ( item->findAndGetFloat64( DCM_PhysicalDeltaY, 
                                          tmpDouble ).good() )
            {

              _physicalDeltaY = 10.0 * std::fabs( tmpDouble );
              status = true;

            }

          }

        }

      }

      return status;

    }

  }

  return false;

}


double soma::USRegionSpatialModule::getPhysicalDeltaX() const
{

  return _physicalDeltaX;

}


double soma::USRegionSpatialModule::getPhysicalDeltaY() const
{

  return _physicalDeltaY;

}
