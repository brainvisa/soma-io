#include <soma-io/Dicom/USReader.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcsequen.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <cmath>


soma::USReader::USReader()
              : soma::DicomReader()
{
}


bool soma::USReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    bool status = true;

    if ( ( m_dataInfo.m_pixelSpacingX <= 0.0 ) ||
         ( m_dataInfo.m_pixelSpacingY <= 0.0 ) )
    {

      int32_t nScales = 0;
      Float64 tmpFD = 0.0;
      DcmSequenceOfItems* seq = 0;

      if ( dataset->findAndGetSequence( DCM_SequenceOfUltrasoundRegions, 
                                        seq ).good() )
      {

        Uint16 type;
        bool found = false;
        uint32_t i, nItems = seq->card();

        for ( i = 0; !found && ( i < nItems ); i++ )
        {

          DcmItem* item = seq->getItem( i );

          if ( item->findAndGetUint16( DCM_RegionSpatialFormat, type ).good() )
          {

    	    if ( type == 1 )
    	    {

    	      if ( item->findAndGetFloat64( DCM_PhysicalDeltaX, tmpFD ).good() )
	      {

                m_dataInfo.m_pixelSpacingX = 10.0 * std::fabs( tmpFD );
		nScales++;

	      }
	    
    	      if ( item->findAndGetFloat64( DCM_PhysicalDeltaY, tmpFD ).good() )
	      {

                m_dataInfo.m_pixelSpacingY = 10.0 * std::fabs( tmpFD );
		nScales++;

	      }

              if ( nScales == 2 )
              {

                found = true;

              }

    	    }

          }

        }

      }

      if ( nScales != 2 )
      {

        status = false;

      }

    }

    return status;

  }

  return false;

}
