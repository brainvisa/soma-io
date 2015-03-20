#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Bruker/BrukerEnhancedMRReader.h>
#include <soma-io/Dicom/Bruker/BrukerEnhancedIndexModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Bruker/BrukerEnhancedMRReader.h>
#include <Dicom/Bruker/BrukerEnhancedIndexModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/DicomReaderFactory.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcsequen.h>


soma::BrukerEnhancedMRReader::BrukerEnhancedMRReader()
                            : soma::EnhancedMRImageStorageReader(),
                              soma::Singleton< soma::BrukerEnhancedMRReader >()
{
}


soma::BrukerEnhancedMRReader::~BrukerEnhancedMRReader()
{
}


std::string soma::BrukerEnhancedMRReader::getManufacturerName()
{

  return "Bruker BioSpin MRI GmbH";

}


bool soma::BrukerEnhancedMRReader::buildIndexLut( DcmDataset* dataset )
{

  if ( dataset )
  {

    soma::BrukerEnhancedIndexModule indexModule;

    if ( indexModule.parseItem( dataset ) )
    {

      _dataInfo->_slices = indexModule.getZCount();
      _dataInfo->_frames = indexModule.getTCount();
      _indexLut = indexModule.getIndices();

      return true;

    }

  }

  return false;

}


RegisterDicomReaderFunction( BrukerEnhancedMRReader );
