#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>

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


soma::DicomDatasetHeader::DicomDatasetHeader( soma::DicomProxy& proxy )
                        : m_binaryHeader( proxy.getBinaryHeader() )
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

    int32_t length = dataset->calcElementLength( EXS_LittleEndianExplicit,
                                                 EET_ExplicitLength );
    
    if ( m_binaryHeader.allocate( i, length ) )
    {

      DcmOutputBufferStream dobs( m_binaryHeader[ i ].first,
                                  m_binaryHeader[ i ].second  );

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

  if ( ( i >= 0 ) && ( i < m_binaryHeader.getCount() ) )
  {

    if ( m_binaryHeader[ i ].second )
    {

      DcmInputBufferStream dibs;

      dibs.setBuffer( (const void*)m_binaryHeader[ i ].first,
                      m_binaryHeader[ i ].second );
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

  dictionary.clear();

  int32_t slice = 0;
  soma::BinaryHeader::iterator
    d = m_binaryHeader.begin(),
    de = m_binaryHeader.end();

  while ( d != de )
  {

    std::ostringstream ossSlice;

    ossSlice << slice++ << std::flush;

    std::ostringstream ossDataset;
    int32_t length = d->second;
    uint8_t* p = d->first;

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

  if ( int32_t( dictionary.size() ) != m_binaryHeader.getCount() )
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

    if ( m_binaryHeader.allocate( slice, length ) )
    {

      std::istringstream issDataset( d->second );
      uint8_t* p = m_binaryHeader[ slice ].first;
      int32_t value;

      while ( length-- )
      {

        issDataset >> std::setbase( 16 ) >> value;
        *p++ = (uint8_t)value;

      }

    }

    ++d;

  }

  return true;

}
