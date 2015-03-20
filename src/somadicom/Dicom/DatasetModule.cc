#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DatasetModule.h>
#else
#include <Dicom/DatasetModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::DatasetModule::DatasetModule()
                   : soma::DicomModule(),
                     _manufacturer( "Generic" ),
                     _sopClassUID( "" ),
                     _seriesInstanceUID( "" ),
                     _samplesPerPixel( 0 ),
                     _bitsAllocated( 0 )
{
}


bool soma::DatasetModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    OFString tmpString;
    Uint16 tmpShort;

    _manufacturer = "Generic";

    if ( dcmItem->findAndGetOFString( DCM_Manufacturer, tmpString ).good() )
    {

      _manufacturer = tmpString.c_str();

    }

    if ( dcmItem->findAndGetOFString( DCM_SOPClassUID, tmpString ).good() )
    {

      _sopClassUID = tmpString.c_str();

    }

    if ( dcmItem->findAndGetUint16( DCM_SamplesPerPixel, tmpShort ).good() )
    {

      _samplesPerPixel = int32_t( tmpShort );

    }

    if ( dcmItem->findAndGetOFString( DCM_PhotometricInterpretation, 
                                      tmpString ).good() )
    {

      if ( !tmpString.compare( "PALETTE COLOR" ) )
      {

        _samplesPerPixel = 3;

      }

    }

    if ( dcmItem->findAndGetUint16( DCM_BitsAllocated, tmpShort ).good() )
    {

      _bitsAllocated = int32_t( tmpShort );

    }

    if ( dcmItem->findAndGetOFString( DCM_SeriesInstanceUID, 
                                      tmpString ).good() )
    {

      _seriesInstanceUID = tmpString.c_str();

      return true;

    }

  }

  return false;

}


const std::string& soma::DatasetModule::getManufacturer() const
{

  return _manufacturer;

}


const std::string& soma::DatasetModule::getSOPClassUID() const
{

  return _sopClassUID;

}


const std::string& soma::DatasetModule::getSeriesInstanceUID() const
{

  return _seriesInstanceUID;

}


int32_t soma::DatasetModule::getSamplesPerPixel() const
{

  return _samplesPerPixel;

}


int32_t soma::DatasetModule::getBitsAllocated() const
{

  return _bitsAllocated;

}
