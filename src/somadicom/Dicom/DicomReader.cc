#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomReader.h>
#include <soma-io/Dicom/DatasetModule.h>
#include <soma-io/Dicom/IdentifyingModule.h>
#include <soma-io/Dicom/PatientModule.h>
#include <soma-io/Dicom/RelationshipModule.h>
#include <soma-io/Dicom/ImageModule.h>
#include <soma-io/Dicom/ModalityLutModule.h>
#include <soma-io/Dicom/AcquisitionModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomReader.h>
#include <Dicom/DatasetModule.h>
#include <Dicom/IdentifyingModule.h>
#include <Dicom/PatientModule.h>
#include <Dicom/RelationshipModule.h>
#include <Dicom/ImageModule.h>
#include <Dicom/ModalityLutModule.h>
#include <Dicom/AcquisitionModule.h>
#include <Dicom/DicomDatasetHeader.h>
#include <System/DirectoryParser.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <sstream>
#include <ctime>

#ifdef MINI_VIEWER
#include <sys/time.h>
#include <iostream>
#endif


dcm::DicomReader::DicomReader()
                : _dataInfo( 0 )
{
}


dcm::DicomReader::~DicomReader()
{
}


std::string dcm::DicomReader::getManufacturerName()
{

  return std::string( "Generic" );

}


bool dcm::DicomReader::check( const dcm::DatasetModule& datasetModule,
                              dcm::DirectoryParser& directory,
                              dcm::DataInfo& dataInfo,
                              dcm::DicomDatasetHeader& datasetHeader )
{

#ifdef MINI_VIEWER
  struct timeval tv_start;
  struct timeval tv_1;
  struct timeval tv_2;
  struct timeval tv_3;

  gettimeofday( &tv_start, NULL );
#endif

  bool noFlip = dataInfo._noFlip;
  bool noDemosaic = dataInfo._noDemosaic;
  bool modalityLut = dataInfo._modalityLut;
  std::string selectedFile = directory.getSelectedFile();

  _dataInfo = &dataInfo;
  _dataInfo->clear();
  _dataInfo->_noFlip = noFlip;
  _dataInfo->_noDemosaic = noDemosaic;
  _dataInfo->_modalityLut = modalityLut;

  if ( selectFiles( directory, 
                    datasetModule.getSeriesInstanceUID(), 
                    datasetHeader ) )
  {

#ifdef MINI_VIEWER
    gettimeofday( &tv_1, NULL );
#endif

    if ( readHeader( datasetHeader ) )
    {

#ifdef MINI_VIEWER
      gettimeofday( &tv_2, NULL );
#endif

      if ( sortFiles( datasetHeader ) )
      {

#ifdef MINI_VIEWER
        gettimeofday( &tv_3, NULL );

        double d1 = (double)( tv_1.tv_sec + tv_1.tv_usec / 1000000.0 - 
                              tv_start.tv_sec - tv_start.tv_usec / 1000000.0 );
        double d2 = (double)( tv_2.tv_sec + tv_2.tv_usec / 1000000.0 - 
                              tv_1.tv_sec - tv_1.tv_usec / 1000000.0 );
        double d3 = (double)( tv_3.tv_sec + tv_3.tv_usec / 1000000.0 - 
                              tv_2.tv_sec - tv_2.tv_usec / 1000000.0 );
        double dT = (double)( tv_3.tv_sec + tv_3.tv_usec / 1000000.0 - 
                              tv_start.tv_sec - tv_start.tv_usec / 1000000.0 );
        std::cout << "Check time : " << dT << " s " << std::endl;
        std::cout << "  - file selection  : " << d1 << " s" << std::endl;
        std::cout << "  - header analysis : " << d2 << " s" << std::endl;
        std::cout << "  - sort files      : " << d3 << " s" << std::endl;
#endif

        _dataInfo->initialize();

        return true;

      }

    }

  }

  return false;

}


bool dcm::DicomReader::getHeader( dcm::HeaderProxy& header, 
                                  dcm::DataInfo& info,
                                  dcm::DicomDatasetHeader& datasetHeader )
{

  header.addAttribute( "sizeX", info._width );
  header.addAttribute( "sizeY", info._height );
  header.addAttribute( "sizeZ", info._slices );
  header.addAttribute( "sizeT", info._frames );

  header.addAttribute( "resolutionX", info._resolution.x );
  header.addAttribute( "resolutionY", info._resolution.y );
  header.addAttribute( "resolutionZ", info._resolution.z );
  header.addAttribute( "resolutionT", info._repetitionTime );

  dcm::IdentifyingModule identifyingModule;

  if ( identifyingModule.parseHeader( datasetHeader ) )
  {

    header.addAttribute( "study_date", identifyingModule.getStudyDate() );
    header.addAttribute( "series_date", identifyingModule.getSeriesDate() );
    header.addAttribute( "acquisition_date", 
                         identifyingModule.getAcquisitionDate() );
    header.addAttribute( "study_time", identifyingModule.getStudyTime() );
    header.addAttribute( "series_time", identifyingModule.getSeriesTime() );
    header.addAttribute( "acquisition_time", 
                         identifyingModule.getAcquisitionTimes() );
    header.addAttribute( "modality", identifyingModule.getModality() );
    header.addAttribute( "manufacturer", identifyingModule.getManufacturer() );
    header.addAttribute( "institution_name", 
                         identifyingModule.getInstitutionName() );
    header.addAttribute( "series_description", 
                         identifyingModule.getSeriesDescription() );
    header.addAttribute( "zero_start_time", 
                         identifyingModule.getZeroStartTime() );

  }

  DcmDataset dataset;
  dcm::PatientModule patientModule;
  dcm::RelationshipModule relationshipModule;
  dcm::ModalityLutModule modalityLutModule;

  datasetHeader.get( dataset );

  if ( patientModule.parseItem( &dataset ) )
  {

    header.addAttribute( "patient_name", patientModule.getName() );
    header.addAttribute( "patient_id", patientModule.getId() );

  }

  if ( relationshipModule.parseHeader( datasetHeader ) )
  {

    header.addAttribute( "series_instance_uid", 
                         relationshipModule.getSeriesInstanceUID() );
    header.addAttribute( "study_id", relationshipModule.getStudyId() );
    header.addAttribute( "series_number", 
                         relationshipModule.getSeriesNumber() );
    header.addAttribute( "acquisition_number", 
                         relationshipModule.getAcquisitionNumber() );

  }

  if ( !info._modalityLut && modalityLutModule.parseHeader( datasetHeader ) )
  {

    header.addAttribute( "rescale_intercept",  
                         modalityLutModule.getRescaleIntercept() );
    header.addAttribute( "rescale_slope", 
                         modalityLutModule.getRescaleSlope() );

  }

  return true;

}


bool dcm::DicomReader::read( dcm::DicomDatasetHeader& datasetHeader, 
                             dcm::DicomProxy& proxy )
{

  if ( datasetHeader.size() )
  {

#ifdef MINI_VIEWER
    struct timeval tv_start;
    struct timeval tv_stop;

    gettimeofday( &tv_start, NULL );
#endif

    // read dataset
    bool status = readData( datasetHeader, proxy );

#ifdef MINI_VIEWER
    gettimeofday( &tv_stop, NULL );

    double d1 = (double)( tv_stop.tv_sec + tv_stop.tv_usec / 1000000.0 - 
                          tv_start.tv_sec - tv_start.tv_usec / 1000000.0 );
    std::cout << "Read time : " << std::endl;
    std::cout << "  - data read       : " << d1 << " s" << std::endl;
#endif

    return status;

  }

  return false;

}


bool dcm::DicomReader::readHeader( dcm::DicomDatasetHeader& datasetHeader )
{

  if ( datasetHeader.size() )
  {

    DcmDataset dataset;
    dcm::ImageModule imageModule;

    datasetHeader.get( dataset );

    if ( imageModule.parseItem( &dataset ) )
    {

      int32_t depth = imageModule.getBitsAllocated();

      _dataInfo->_width = imageModule.getColumns();
      _dataInfo->_height = imageModule.getRows();
      _dataInfo->_resolution.x = imageModule.getPixelSpacingX();
      _dataInfo->_resolution.y = imageModule.getPixelSpacingY();
      _dataInfo->_depth = depth < 8 ? 8 : depth;
      _dataInfo->_spp = imageModule.getSamplesPerPixel();
      _dataInfo->_bitsStored = imageModule.getBitsStored();
      _dataInfo->_pixelRepresentation = imageModule.getPixelRepresentation();

    }
    else
    {

      return false;

    }

    dcm::AcquisitionModule acquisitionModule;

    if ( acquisitionModule.parseItem( &dataset ) )
    {

      if ( acquisitionModule.getSpacingBetweenSlices() > 0.0 )
      {

        _dataInfo->_spacingBetweenSlices = 
                                    acquisitionModule.getSpacingBetweenSlices();

      }
      else if ( acquisitionModule.getSliceThickness() > 0.0 )
      {

        _dataInfo->_spacingBetweenSlices = 
                                          acquisitionModule.getSliceThickness();

      }

    }

    _dataInfo->_resolution.z = _dataInfo->_spacingBetweenSlices;
    _dataInfo->_minimum = float( ( 1 << _dataInfo->_bitsStored ) - 1 );
    _dataInfo->_maximum = float( 1 - ( 1 << _dataInfo->_bitsStored ) );

    return readHeader( &dataset );

  }

  return false;

}


void dcm::DicomReader::setOrientation()
{

  _dataInfo->_patientOrientation.set( _dataInfo->_rowVec,
                                      _dataInfo->_colVec,
                                      _dataInfo->_origin,
                                      _dataInfo->_width,
                                      _dataInfo->_height,
                                      _dataInfo->_slices,
                                      _dataInfo->_resolution );

  if ( !_dataInfo->_noFlip )
  {

    dcm::Vector3d< int32_t > sizes = _dataInfo->_patientOrientation.getSize();

    _dataInfo->_width = sizes.x;
    _dataInfo->_height = sizes.y;
    _dataInfo->_slices = sizes.z;
    _dataInfo->_resolution = _dataInfo->_patientOrientation.getResolution();

  }

}


bool dcm::DicomReader::selectFiles( dcm::DirectoryParser& directory,
                                    const std::string& /* seriesInstanceUID */,
                                    dcm::DicomDatasetHeader& datasetHeader )
{

  std::string fileName = directory.getSelectedFile();

  if ( fileName.size() )
  {

    DcmFileFormat header;

    if ( header.loadFile( fileName.c_str() ).good() )
    {

      datasetHeader.clear();

      if ( datasetHeader.add( header.getDataset(), fileName ) )
      {

        _dataInfo->_fileCount = 1;

        return true;

      }

    }

  }

  return false;

}


bool dcm::DicomReader::sortFiles( dcm::DicomDatasetHeader& /* datasetHeader */ )
{

  setOrientation();

  return true;

}


bool dcm::DicomReader::readHeader( DcmDataset* /* dataset */ )
{

  return true;

}
