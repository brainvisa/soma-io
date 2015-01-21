#include <soma-io/Dicom/Siemens/SiemensMRReader.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Dicom/Siemens/Demosaicer.h>
#include <soma-io/Dicom/Siemens/MosaicDataContext.h>
#include <soma-io/Dicom/DicomReaderFactory.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/System/DirectoryParser.h>
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


bool soma::SiemensMRReader::getHeader( soma::HeaderProxy& proxy,
                                       soma::DataInfo& info,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  if ( !soma::MRImageStorageReader::getHeader( proxy, info, datasetHeader ) )
  {

    return false;

  }

  if ( !proxy.hasAttribute( "b_values" ) )
  {

    DcmDataset dataset;
    Sint32 tmpInt;

    datasetHeader.get( dataset );

    if ( dataset.findAndGetSint32( DcmTagKey( 0x0019, 0x100c ), 
                                   tmpInt ).good() )
    {

      int32_t i, n = datasetHeader.size();
      std::vector< double > bValues;
      std::vector< std::vector< double > > directions;
      Float64 tmpFloat;

      for ( i = 0; i < n; i++ )
      {

        datasetHeader.get( dataset, i );

        if ( dataset.findAndGetSint32( DcmTagKey( 0x0019, 0x100c ),
                                       tmpInt ).good() )
        {

          bValues.push_back( double( tmpInt ) );

        }

        std::vector< double > direction( 3, 0.0 );

        if ( tmpInt > 0 )
        {

          int32_t d;

          for ( d = 0; d < 3; d++ )
          {

            if ( dataset.findAndGetFloat64( DcmTagKey( 0x0019, 0x100e ),
                                            tmpFloat,
                                            d ).good() )
            {

              direction[ d ] = double( tmpFloat );

            }

          }

          direction[ 2 ] *= -1.0;

        }

        directions.push_back( direction );

      }

      if ( bValues.size() )
      {

        proxy.addAttribute( "b_values", bValues );

      }

      if ( directions.size() )
      {

        proxy.addAttribute( "diffusion_directions", directions );

      }

    }

  }

  return true;

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


std::vector< std::string > soma::SiemensMRReader::sortFiles( 
                                              soma::DirectoryParser& directory )
{

  std::vector< std::string > files = soma::MultiFileReader::sortFiles( 
                                                                    directory );

  if ( m_demosaicer )
  {

    soma::Vector origin = m_demosaicer->demosaic( m_origin );
    m_dataInfo->m_patientOrientation.setOrigin( origin );

  }

  return files;

}


RegisterDicomReaderFunction( SiemensMRReader );
