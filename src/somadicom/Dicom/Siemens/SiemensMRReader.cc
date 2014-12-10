#include <soma-io/Dicom/Siemens/SiemensMRReader.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Container/DicomProxy.h>
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

  if ( dataset )
  {

    OFString imageType;

    if ( m_demosaicer )
    {

      delete m_demosaicer;

      m_demosaicer = 0;

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

          m_sliceCount = int32_t( sliceCount );

        }

        m_demosaicer = new soma::Demosaicer(
                                           m_dataInfo->m_rowVec,
                                           m_dataInfo->m_colVec,
                                           m_dataInfo->m_width,
                                           m_dataInfo->m_height,
                                           m_dataInfo->m_slices,
                                           m_sliceCount,
                                           m_dataInfo->m_resolution.x,
                                           m_dataInfo->m_resolution.y,
                                           m_dataInfo->m_spacingBetweenSlices );

        if ( !m_demosaicer )
        {

          return false;

        }

        if ( !m_dataInfo->m_noDemosaic )
        {

          m_dataInfo->m_mosaic = true;
          m_demosaicer->getSize( m_dataInfo->m_width, 
                                 m_dataInfo->m_height,
                                 m_dataInfo->m_slices );

        }

      }

    }

    return soma::MRImageStorageReader::readHeader( dataset );

  }

  return false;

}


bool soma::SiemensMRReader::readData( soma::DicomProxy& proxy, 
                                      soma::Callback* progress )
{

  if ( !m_demosaicer || ( m_sliceCount == 1 ) )
  {

    return soma::MRImageStorageReader::readData( proxy, progress );

  }

  soma::Vector origin = m_demosaicer->demosaic( m_origin );
  proxy.getDataInfo().m_patientOrientation.setOrigin( origin );

  if ( proxy.allocate() )
  {

    soma::MosaicDataContext context( m_slices, 
                                     proxy, 
                                     *m_demosaicer,
                                     progress );
    carto::ThreadedLoop threadedLoop( &context,
                                      0,
                                      int32_t( m_slices.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}


RegisterDicomReaderFunction( SiemensMRReader );
