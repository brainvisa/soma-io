#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/IdentifyingModule.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#else
#include <Dicom/IdentifyingModule.h>
#include <Dicom/DicomDatasetHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <algorithm>


dcm::IdentifyingModule::IdentifyingModule()
                      : dcm::DicomModule(),
                        _studyDate( "" ),
                        _seriesDate( "" ),
                        _acquisitionDate( "" ),
                        _studyTime( "" ),
                        _seriesTime( "" ),
                        _modality( "" ),
                        _manufacturer( "" ),
                        _institutionName( "" ),
                        _seriesDescription( "" ),
                        _zeroStartTime( 0 )
{
}


bool dcm::IdentifyingModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;

    if ( dcmItem->findAndGetOFString( DCM_StudyDate, tmpString ).good() )
    {

      _studyDate = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesDate, tmpString ).good() )
    {

      _seriesDate = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_AcquisitionDate, tmpString ).good() )
    {

      _acquisitionDate = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_StudyTime, tmpString ).good() )
    {

      _studyTime = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesTime, tmpString ).good() )
    {

      _seriesTime = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_Modality, tmpString ).good() )
    {

      _modality = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_Manufacturer, tmpString ).good() )
    {

      _manufacturer = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_InstitutionName, tmpString ).good() )
    {

      _institutionName = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesDescription, 
                                      tmpString ).good() )
    {

      _seriesDescription = tmpString.c_str();

    }

    return true;

  }

  return false;

}


bool dcm::IdentifyingModule::parseHeader( 
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  if ( n )
  {

    DcmDataset dataset;
    OFString tmpString;
    int32_t i;

    _acquisitionTimes.clear();
    datasetHeader.get( dataset );

    if ( parseItem( &dataset ) )
    {

      for ( i = 0; i < n; i++ )
      {

        datasetHeader.get( dataset, i );

        if ( dataset.findAndGetOFString( DCM_AcquisitionTime, 
                                         tmpString ).good() )
        {

          _acquisitionTimes.push_back( tmpString.c_str() );

        }

      }

      if ( _acquisitionTimes.size() )
      {

        std::string timeStr = _acquisitionTimes[ 0 ];

        if ( timeStr.size() )
        {

          struct tm t;
          std::istringstream iss;

          iss.str( timeStr.substr( 0, 2 ) );
          iss >> t.tm_hour;
          iss.clear();
          iss.str( timeStr.substr( 2, 2 ) );
          iss >> t.tm_min;
          iss.clear();
          iss.str( timeStr.substr( 4, 2 ) );
          iss >> t.tm_sec;

          if ( _acquisitionDate.size() )
          {

            t.tm_isdst = -1;

            iss.clear();
            iss.str( _acquisitionDate.substr( 0, 4 ) );
            iss >> t.tm_year;
            iss.clear();
            iss.str( _acquisitionDate.substr( 4, 2 ) );
            iss >> t.tm_mon;
            iss.clear();
            iss.str( _acquisitionDate.substr( 6, 2 ) );
            iss >> t.tm_mday;

            _zeroStartTime = int32_t( mktime( &t ) );

          }
          else
          {

            _zeroStartTime = ( t.tm_hour * 60 + t.tm_min ) * 60 + t.tm_sec;

          }

          size_t nEqual = std::count( _acquisitionTimes.begin(),
                                      _acquisitionTimes.end(),
                                      timeStr );

          if ( nEqual == _acquisitionTimes.size() )
          {

            _acquisitionTimes.resize( 1 );

          }

        }

      }

      return true;

    }

  }

  return false;

}


const std::string& dcm::IdentifyingModule::getStudyDate() const
{

  return _studyDate;

}


const std::string& dcm::IdentifyingModule::getSeriesDate() const
{

  return _seriesDate;

}


const std::string& dcm::IdentifyingModule::getAcquisitionDate() const
{

  return _acquisitionDate;

}


const std::string&dcm::IdentifyingModule:: getStudyTime() const
{

  return _studyTime;

}


const std::string&dcm::IdentifyingModule:: getSeriesTime() const
{

  return _seriesTime;

}


const std::vector< std::string >& 
dcm::IdentifyingModule::getAcquisitionTimes() const
{

  return _acquisitionTimes;

}


const std::string& dcm::IdentifyingModule::getModality() const
{

  return _modality;

}


const std::string& dcm::IdentifyingModule::getManufacturer() const
{

  return _manufacturer;

}


const std::string& dcm::IdentifyingModule::getInstitutionName() const
{

  return _institutionName;

}


const std::string& dcm::IdentifyingModule::getSeriesDescription() const
{

  return _seriesDescription;

}


int32_t dcm::IdentifyingModule::getZeroStartTime() const
{

  return _zeroStartTime;

}
