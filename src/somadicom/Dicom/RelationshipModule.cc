#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/RelationshipModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/RelationshipModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <algorithm>


soma::RelationshipModule::RelationshipModule()
                        : soma::DicomModule(),
                          _studyId( "" ),
                          _seriesNumber( 0 )
{
}


bool soma::RelationshipModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;
    Sint32 tmpInt;

    if ( dcmItem->findAndGetOFString( DCM_SeriesInstanceUID, tmpString ).good() )
    {

      _seriesInstanceUID = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_StudyID, tmpString ).good() )
    {

      _studyId = tmpString.c_str();

    }

    if ( dcmItem->findAndGetSint32( DCM_SeriesNumber, tmpInt ).good() )
    {

      _seriesNumber = int32_t( tmpInt );

    }

    return true;

  }

  return false;

}


bool soma::RelationshipModule::parseHeader( 
                                       soma::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    Sint32 tmpInt;
    int32_t i;

    _acquisitionNumber.clear();
    datasetHeader.get( dataset );

    if ( parseItem( &dataset ) )
    {

      for ( i = 0; i < n; i++ )
      {

        datasetHeader.get( dataset, i );

        if ( dataset.findAndGetSint32( DCM_AcquisitionNumber, tmpInt ).good() )
        {

          _acquisitionNumber.push_back( int32_t( tmpInt ) );

        }

      }

      size_t nEqual = std::count( _acquisitionNumber.begin(),
                                  _acquisitionNumber.end(),
                                  _acquisitionNumber[ 0 ] );

      if ( nEqual == _acquisitionNumber.size() )
      {

        _acquisitionNumber.resize( 1 );

      }

      return true;

    }

  }

  return false;

}


const std::string& soma::RelationshipModule::getSeriesInstanceUID() const
{

  return _seriesInstanceUID;

}


const std::string& soma::RelationshipModule::getStudyId() const
{

  return _studyId;

}


int32_t soma::RelationshipModule::getSeriesNumber() const
{

  return _seriesNumber;

}


const std::vector< int32_t >& 
soma::RelationshipModule::getAcquisitionNumber() const
{

  return _acquisitionNumber;

}

