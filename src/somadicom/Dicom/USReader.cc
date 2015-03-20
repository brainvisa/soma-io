#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USReader.h>
#include <soma-io/Dicom/USRegionSpatialModule.h>
#else
#include <Dicom/USReader.h>
#include <Dicom/USRegionSpatialModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


soma::USReader::USReader()
              : soma::DicomReader()
{
}


bool soma::USReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    if ( ( _dataInfo->_resolution.x <= 0.0 ) ||
         ( _dataInfo->_resolution.y <= 0.0 ) )
    {

      soma::USRegionSpatialModule spatialModule;

      if ( spatialModule.parseItem( dataset ) )
      {

        _dataInfo->_resolution.x = spatialModule.getPhysicalDeltaX();
        _dataInfo->_resolution.y = spatialModule.getPhysicalDeltaY();

        return true;

      }

      return false;

    }

    return true;

  }

  return false;

}
