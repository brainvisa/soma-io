#include <soma-io/Dicom/Siemens/SiemensMRReader.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Container/Data.h>
#include <soma-io/Pattern/Callback.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcuid.h>


soma::SiemensMRReader::SiemensMRReader()
                     : soma::MRImageStorageReader(),
                       soma::Singleton< soma::SiemensMRReader >(),
                       m_demosaicer( 0 ),
                       m_mosaic( false ),
                       m_sliceCount( 1 )
{
}


soma::SiemensMRReader::~SiemensMRReader()
{

  delete m_demosaicer;

}


std::string soma::SiemensMRReader::getManufacturerName()
{

  return "SIEMENS";

}


bool soma::SiemensMRReader::readHeader( DcmDataset* dataset )
{

  m_mosaic = false;

  if ( dataset )
  {

    OFString imageType;

    if ( dataset->findAndGetOFStringArray( DCM_ImageType,
                                           imageType ).good() )
    {

      if ( imageType.find( "MOSAIC" ) != std::string::npos )
      {

        m_mosaic = true;

        Uint16 sliceCount;
        if ( dataset->findAndGetUint16( DcmTagKey( 0x0019, 0x100a ),
                                        sliceCount ).good() )
        {

          m_sliceCount = int32_t( sliceCount );

        }

      }

    }

    if ( m_demosaicer )
    {

      delete m_demosaicer;

    }

    m_demosaicer = new soma::Demosaicer( m_dataInfo.m_rowVec,
                                         m_dataInfo.m_colVec,
                                         m_dataInfo.m_width,
                                         m_dataInfo.m_height,
                                         m_dataInfo.m_slices,
                                         m_sliceCount,
                                         m_dataInfo.m_pixelSpacingX,
                                         m_dataInfo.m_pixelSpacingY,
                                         m_dataInfo.m_spacingBetweenSlices );

    m_demosaicer->getSize( m_dataInfo.m_width, 
                           m_dataInfo.m_height,
                           m_dataInfo.m_slices );

    return soma::MRImageStorageReader::readHeader( dataset );

  }

  return false;

}


bool soma::SiemensMRReader::readData( soma::Data& data, 
                                      soma::Callback* progress )
{

  if ( !m_mosaic || ( m_sliceCount == 1 ) )
  {

    return soma::MRImageStorageReader::readData( data, progress );

  }

  if ( m_demosaicer )
  {

    soma::DataInfo dataInfo( m_dataInfo );

    soma::Vector origin = m_demosaicer->demosaic( m_origin );
    dataInfo.m_patientOrientation.setOrigin( origin );

    if ( data.Create( dataInfo ) )
    {

      soma::MosaicDataContext context( m_slices, 
                                       data, 
                                       *m_demosaicer,
                                       progress );
      carto::ThreadedLoop threadedLoop( &context,
                                        0,
                                        int32_t( m_slices.size() ) );

      threadedLoop.launch();

      return true;

    }

  }

  return false;

}


RegisterDicomReaderFunction( SiemensMRReader );
