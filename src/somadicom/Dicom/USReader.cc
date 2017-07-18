#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/USReader.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Dicom/USRegionSpatialModule.h>
#else
#include <Dicom/USReader.h>
#include <Container/DataInfo.h>
#include <Dicom/USRegionSpatialModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>


dcm::USReader::USReader()
{
}


dcm::USReader::~USReader()
{
}


bool dcm::USReader::readHeader( DcmDataset* dataset, dcm::DataInfo& dataInfo )
{

  if ( dataset )
  {

    dcm::USRegionSpatialModule spatialModule;

    if ( spatialModule.parseItem( dataset ) )
    {

      dataInfo._resolution.x = spatialModule.getPhysicalDeltaX();
      dataInfo._resolution.y = spatialModule.getPhysicalDeltaY();

      return true;

    }

  }

  return false;

}
