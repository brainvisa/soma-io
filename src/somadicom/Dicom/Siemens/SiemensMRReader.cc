#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/SiemensMRReader.h>
#include <soma-io/Dicom/Siemens/SiemensDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/System/DirectoryParser.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/Siemens/SiemensMRReader.h>
#include <Dicom/Siemens/SiemensDiffusionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Dicom/Siemens/Demosaicer.h>
#include <Dicom/Siemens/MosaicDataContext.h>
#include <Dicom/DicomReaderFactory.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <System/DirectoryParser.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::SiemensMRReader::SiemensMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::SiemensMRReader >(),
                       _demosaicer( 0 ),
                       _sliceCount( 1 )
{
}


soma::SiemensMRReader::~SiemensMRReader()
{

  delete _demosaicer;

}


std::string soma::SiemensMRReader::getManufacturerName()
{

  return "SIEMENS";

}


bool soma::SiemensMRReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    soma::SiemensDiffusionModule diffusionModule;

    if ( diffusionModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "b_values", diffusionModule.getBValues() );
      proxy.addAttribute( "diffusion_directions", 
                          diffusionModule.getDirections() );

    }

  }

  return true;

}


bool soma::SiemensMRReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    OFString imageType;

    if ( _demosaicer )
    {

      delete _demosaicer;

      _demosaicer = 0;

    }

    if ( dataset->findAndGetOFStringArray( DCM_ImageType,
                                           imageType ).good() )
    {

      if ( imageType.find( "MOSAIC" ) != std::string::npos )
      {

        Uint16 sliceCount;
        if ( dataset->findAndGetUint16( DcmTagKey( 0x0019, 0x100a ),
                                        sliceCount ).good() )
        {

          _sliceCount = int32_t( sliceCount );

        }

        _demosaicer = new soma::Demosaicer( _dataInfo->_rowVec,
                                            _dataInfo->_colVec,
                                            _dataInfo->_width,
                                            _dataInfo->_height,
                                            _dataInfo->_slices,
                                            _sliceCount,
                                            _dataInfo->_resolution.x,
                                            _dataInfo->_resolution.y,
                                            _dataInfo->_spacingBetweenSlices );

        if ( !_demosaicer )
        {

          return false;

        }

        if ( !_dataInfo->_noDemosaic )
        {

          _dataInfo->_mosaic = true;
          _demosaicer->getSize( _dataInfo->_width, 
                                _dataInfo->_height,
                                _dataInfo->_slices );

        }

      }

    }

    return soma::MRImageStorageReader::readHeader( dataset );

  }

  return false;

}


bool soma::SiemensMRReader::readData( soma::DicomDatasetHeader& datasetHeader,
                                      soma::DicomProxy& proxy )
{

  if ( !_demosaicer || ( _sliceCount == 1 ) )
  {

    return soma::MRImageStorageReader::readData( datasetHeader, proxy );

  }

  if ( proxy.allocate() )
  {

    soma::DataInfo& info = proxy.getDataInfo();
    int32_t startT = info._boundingBox.getLowerT();
    int32_t frameCount = info._boundingBox.getUpperT() - startT + 1;

    soma::MosaicDataContext context( datasetHeader, proxy, *_demosaicer );
    soma::ThreadedLoop threadedLoop( &context, startT, frameCount );

    threadedLoop.launch();

    return true;

  }

  return false;

}


bool soma::SiemensMRReader::sortFiles( soma::DicomDatasetHeader& datasetHeader )
{

  bool status = soma::MultiFileReader::sortFiles( datasetHeader );

  if ( _demosaicer )
  {

    soma::Vector origin = _demosaicer->demosaic( _origin );
    _dataInfo->_patientOrientation.setOrigin( origin );

  }

  return status;

}


RegisterDicomReaderFunction( SiemensMRReader );
