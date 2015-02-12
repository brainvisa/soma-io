#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/ModalityLutModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/ModalityLutModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <algorithm>


soma::ModalityLutModule::ModalityLutModule()
                       : soma::DicomModule()
{
}


bool soma::ModalityLutModule::parseDataset( DcmDataset* dataset )
{

  if ( dataset )
  {

    Float64 tmpDouble;
    double intercept = 0.0;
    double slope = 1.0;

    if ( dataset->findAndGetFloat64( DCM_RescaleIntercept, tmpDouble ).good() )
    {

      intercept = double( tmpDouble );

    }

    if ( dataset->findAndGetFloat64( DCM_RescaleSlope, tmpDouble ).good() )
    {

      slope = double( tmpDouble );

    }

    _rescaleIntercept.push_back( intercept );
    _rescaleSlope.push_back( slope );

    return true;

  }

  return false;

}


bool soma::ModalityLutModule::parseHeader( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    int32_t i;

    _rescaleIntercept.clear();
    _rescaleSlope.clear();

    for ( i = 0; i < n; i++ )
    {

      datasetHeader.get( dataset, i );

      if ( !parseDataset( &dataset ) )
      {

        return false;

      }

    }

    size_t nEqualI = std::count( _rescaleIntercept.begin(),
                                 _rescaleIntercept.end(),
                                 _rescaleIntercept[ 0 ] );
    size_t nEqualS = std::count( _rescaleSlope.begin(),
                                 _rescaleSlope.end(),
                                 _rescaleSlope[ 0 ] );

    if ( ( nEqualI  == _rescaleIntercept.size() ) &&
         ( nEqualS  == _rescaleSlope.size() ) )
    {

      _rescaleIntercept.resize( 1 );
      _rescaleSlope.resize( 1 );

    }

    return true;

  }

  return false;

}


const std::vector< double >& 
soma::ModalityLutModule::getRescaleIntercept() const
{

  return _rescaleIntercept;

}


const std::vector< double >& 
soma::ModalityLutModule::getRescaleSlope() const
{

  return _rescaleSlope;

}

