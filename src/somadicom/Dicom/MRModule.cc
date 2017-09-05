#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MRModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/MRModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <algorithm>


dcm::MRModule::MRModule()
             : dcm::DicomModule(),
               _flipAngle( 0.0 )
{
}


bool dcm::MRModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    Float64 tmpDouble;

    if ( dcmItem->findAndGetFloat64( DCM_RepetitionTime, tmpDouble ).good() )
    {

      _tr.push_back( double( tmpDouble ) );

    }

    if ( dcmItem->findAndGetFloat64( DCM_EchoTime, tmpDouble ).good() )
    {

      _te.push_back( double( tmpDouble ) );

    }

    if ( dcmItem->findAndGetFloat64( DCM_InversionTime, tmpDouble ).good() )
    {

      _ti.push_back( double( tmpDouble ) );

    }

    if ( dcmItem->findAndGetFloat64( DCM_FlipAngle, tmpDouble ).good() )
    {

      _flipAngle = double( tmpDouble );

    }

    return true;

  }

  return false;

}


bool dcm::MRModule::parseDataset( DcmDataset* dataset )
{

  return parseItem( dataset );

}


bool dcm::MRModule::parseHeader( dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    int32_t i, step = getStep( datasetHeader );

    _tr.clear();
    _te.clear();
    _ti.clear();

    for ( i = 0; i < n; i += step )
    {

      datasetHeader.get( dataset, i );

      if ( !parseDataset( &dataset ) )
      {

        return false;

      }

    }

    size_t nEqualTR = std::count( _tr.begin(), _tr.end(), _tr[ 0 ] );
    size_t nEqualTE = std::count( _te.begin(), _te.end(), _te[ 0 ] );
    size_t nEqualTI = std::count( _ti.begin(), _ti.end(), _ti[ 0 ] );

    if ( nEqualTR && ( nEqualTR == _tr.size() ) )
    {

      _tr.resize( 1 );

    }

    if ( nEqualTE && ( nEqualTE == _te.size() ) )
    {

      _te.resize( 1 );

    }

    if ( nEqualTI && ( nEqualTI == _ti.size() ) )
    {

      _ti.resize( 1 );

    }

    return true;

  }

  return false;

}


const std::vector< double >& dcm::MRModule::getTR() const
{

  return _tr;

}


const std::vector< double >& dcm::MRModule::getTE() const
{

  return _te;

}


const std::vector< double >& dcm::MRModule::getTI() const
{

  return _ti;

}


double dcm::MRModule::getFlipAngle() const
{

  return _flipAngle;

}


int32_t dcm::MRModule::getStep( dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t step = 1;
  DcmDataset dataset;
  OFString position;

  datasetHeader.get( dataset );

  if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                        position ).good() )
  {

    OFString tmpString;
    int32_t i, tCount = 1, n = datasetHeader.size();

    for ( i = 1; i < n; i++ )
    {

      datasetHeader.get( dataset, i );

      if ( dataset.findAndGetOFStringArray( DCM_ImagePositionPatient,
                                            tmpString ).good() )
      {

        if ( !tmpString.compare( position ) )
        {

          tCount++;

        }

      }

    }

    step = n / tCount;

  }

  return step;

}
