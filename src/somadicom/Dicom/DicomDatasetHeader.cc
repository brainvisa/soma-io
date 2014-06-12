#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/Data.h>

#include <soma-io/Dicom/soma_osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcostrmb.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <sstream>
#include <iomanip>


soma::DicomDatasetHeader::DicomDatasetHeader( soma::Data& data )
                        : m_data( data )
{
}


soma::DicomDatasetHeader::~DicomDatasetHeader()
{
}


void soma::DicomDatasetHeader::add( DcmDataset* dataset, int32_t i )
{

  if ( dataset )
  {

    dataset->remove( DCM_PixelData );

    soma::DatasetHeader* header = &m_data.m_info.m_datasetHeader[ i ];
    int32_t length = dataset->calcElementLength( EXS_LittleEndianExplicit,
                                                 EET_ExplicitLength );
    
    if ( header->allocate( length ) )
    {

      DcmOutputBufferStream dobs( header->getBuffer(), header->getLength() );

      dataset->transferInit();
#if OFFIS_DCMTK_VERSION_NUMBER >= 360
      dataset->write( dobs, 
                      EXS_LittleEndianExplicit, 
                      EET_ExplicitLength,
                      0,
                      EGL_recalcGL,
                      EPD_withoutPadding );
#else
      dataset->write( dobs, 
                      EXS_LittleEndianExplicit, 
                      EET_ExplicitLength,
                      EGL_recalcGL,
                      EPD_withoutPadding );
#endif
      dataset->transferEnd();

    }

  }

}


void soma::DicomDatasetHeader::get( DcmDataset& dataset, int32_t i )
{

  if ( ( i >= 0 ) &&
       ( i < int32_t( m_data.m_info.m_datasetHeader.size() ) ) )
  {

    soma::DatasetHeader* header = &m_data.m_info.m_datasetHeader[ i ];

    if ( header->getLength() )
    {

      DcmInputBufferStream dibs;

      dibs.setBuffer( (const void*)header->getBuffer(),
                      header->getLength() );
      dibs.setEos();
      dataset.transferInit();
      dataset.read( dibs, EXS_LittleEndianExplicit );
      dataset.transferEnd();

    }

  }

}


bool soma::DicomDatasetHeader::getDictionary( 
                              std::map< std::string, std::string >& dictionary )
{

  if ( int32_t( m_data.m_info.m_datasetHeader.size() ) !=
       m_data.m_info.m_fileCount )
  {

    return false;

  }

  dictionary.clear();

  int32_t slice = 0;
  std::vector< soma::DatasetHeader >::iterator
    d = m_data.m_info.m_datasetHeader.begin(),
    de = m_data.m_info.m_datasetHeader.end();

  while ( d != de )
  {

    std::ostringstream ossSlice;

    ossSlice << slice++ << std::flush;

    std::ostringstream ossDataset;
    int32_t length = d->getLength();
    uint8_t* p = d->getBuffer();

    while ( length-- )
    {

      ossDataset << std::setw( 2 ) << std::setbase( 16 ) << std::setfill( '0' )
                 << ( int32_t )*p++ << " ";

    }

    ossDataset << std::setbase( 10 ) << std::flush;

    dictionary.insert( std::make_pair( ossSlice.str(), ossDataset.str() ) );

    ++d;

  }

  return true;

}


bool soma::DicomDatasetHeader::setDictionary( 
                              std::map< std::string, std::string >& dictionary )
{

  if ( int32_t( dictionary.size() ) != 
       int32_t( m_data.m_info.m_datasetHeader.size() ) )
  {

    return false;

  }

  std::map< std::string, std::string >::const_iterator
    d = dictionary.begin(),
    de = dictionary.end();

  while ( d != de )
  {

    std::istringstream issSlice( d->first );
    int32_t length = ( d->second.size() + 1 ) / 3;
    int32_t slice;

    issSlice >> slice;
    m_data.m_info.m_datasetHeader[ slice ].allocate( length );

    std::istringstream issDataset( d->second );
    uint8_t* p = m_data.m_info.m_datasetHeader[ slice ].getBuffer();
    int32_t value;

    while ( length-- )
    {

      issDataset >> std::setbase( 16 ) >> value;
      *p++ = (uint8_t)value;

    }

    ++d;

  }

  return true;

}
