#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/Siemens/SiemensMRReader.h>
#include <soma-io/Dicom/Siemens/SiemensDiffusionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/DicomIO.h>
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
#include <Dicom/DicomIO.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <System/DirectoryParser.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::SiemensMRReader::SiemensMRReader()
                    : dcm::MRImageStorageReader(),
                      dcm::Singleton< dcm::SiemensMRReader >(),
                      _demosaicer( 0 ),
                      _sliceCount( 1 )
{
}


dcm::SiemensMRReader::~SiemensMRReader()
{

  delete _demosaicer;

}


std::string dcm::SiemensMRReader::getManufacturerName()
{

  return "SIEMENS";

}


bool dcm::SiemensMRReader::getHeader( dcm::HeaderProxy& proxy,
                                      dcm::DataInfo& info,
                                      dcm::DicomDatasetHeader& datasetHeader )
{

  if ( !dcm::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    dcm::SiemensDiffusionModule diffusionModule;

    if ( diffusionModule.parseHeader( datasetHeader ) )
    {

      proxy.addAttribute( "b_values", diffusionModule.getBValues() );
      proxy.addAttribute( "diffusion_directions", 
                          diffusionModule.getDirections() );

    }

  }

  if ( info._mosaic )
  {

    proxy.addAttribute( "mosaic", 1 );
    proxy.addAttribute( "slices_in_mosaic", _sliceCount );

  }

  return true;

}


bool dcm::SiemensMRReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    OFString imageType;

    if ( _demosaicer )
    {

      delete _demosaicer;

      _demosaicer = 0;

    }

    _sliceCount = 1;

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

        _demosaicer = new dcm::Demosaicer( _dataInfo->_rowVec,
                                           _dataInfo->_colVec,
                                           _dataInfo->_width,
                                           _dataInfo->_height,
                                           _dataInfo->_slices,
                                           _sliceCount,
                                           _dataInfo->_resolution.x,
                                           _dataInfo->_resolution.y );

        if ( !_demosaicer )
        {

          return false;

        }

        _dataInfo->_mosaic = true;

        if ( !_dataInfo->_noDemosaic )
        {

          _demosaicer->getSize( _dataInfo->_width, 
                                _dataInfo->_height,
                                _dataInfo->_slices );

        }

      }

    }

    return dcm::MRImageStorageReader::readHeader( dataset );

  }

  return false;

}


bool dcm::SiemensMRReader::readData( dcm::DicomDatasetHeader& datasetHeader,
                                     dcm::DicomProxy& proxy )
{

  if ( !_demosaicer || ( _sliceCount == 1 ) )
  {

    return dcm::MRImageStorageReader::readData( datasetHeader, proxy );

  }

  if ( proxy.allocate() )
  {

    dcm::DataInfo& info = proxy.getDataInfo();
    int32_t startT = info._boundingBox.getLowerT();
    int32_t frameCount = info._boundingBox.getUpperT() - startT + 1;

    dcm::MosaicDataContext context( datasetHeader, proxy, *_demosaicer );
#ifdef SOMA_IO_DICOM
    carto::ThreadedLoop threadedLoop( &context, startT, frameCount );
#else
    dcm::ThreadedLoop threadedLoop( &context, startT, frameCount );
#endif

    threadedLoop.launch();

    return true;

  }

  return false;

}


bool dcm::SiemensMRReader::sortFiles( dcm::DicomDatasetHeader& datasetHeader )
{

  bool status = dcm::MultiFileReader::sortFiles( datasetHeader );

  if ( _demosaicer )
  {

    dcm::Vector3d< double > origin = _demosaicer->demosaic( 
                                                           _dataInfo->_origin );
    _dataInfo->_patientOrientation.setOrigin( origin );

  }

  return status;

}


RegisterDicomReaderFunction( SiemensMRReader );
