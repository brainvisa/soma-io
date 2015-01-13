#include <soma-io/Dicom/Philips/PhilipsPTReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::PhilipsPTReader::PhilipsPTReader()
                     : soma::PTImageStorageReader(),
                       soma::Singleton< soma::PhilipsPTReader >()
{
}


soma::PhilipsPTReader::~PhilipsPTReader()
{
}


std::string soma::PhilipsPTReader::getManufacturerName()
{

  return "Philips Medical Systems";

}


bool soma::PhilipsPTReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::PTImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  DcmDataset dataset;
  OFString tmpString;

  datasetHeader.get( dataset );
 
  if ( dataset.findAndGetOFString( DCM_Units, tmpString ).good() )
  {

    if ( !tmpString.compare( "CNTS" ) )
    {

      Float64 tmpDouble;

      if ( dataset.findAndGetFloat64( DcmTagKey( 0x7053, 0x1009 ), 
                                      tmpDouble ).good() )
      {

        proxy.addAttribute( "image_unit", m_unitNames[ "BQML" ] );
        proxy.addAttribute( "scale_factor", double( tmpDouble ) );

      }

    }

  }

  return true;

}


RegisterDicomReaderFunction( PhilipsPTReader );

