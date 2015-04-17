#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/BinaryHeader.h>
#else
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/BinaryHeader.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcostrmb.h>
#include <dcmtk/dcmdata/dcistrmb.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <algorithm>
#include <functional>
#include <sstream>
#include <iomanip>



dcm::DicomDatasetHeader::DicomDatasetHeader( dcm::DataInfo& dataInfo )
                       : _binaryHeader( dataInfo._datasetHeader )
{

  _binaryHeader.setFormat( "dicom" );

}


dcm::DicomDatasetHeader::DicomDatasetHeader( dcm::DicomProxy& proxy )
                       : _binaryHeader( proxy.getBinaryHeader() )
{

  _binaryHeader.setFormat( "dicom" );

}


dcm::DicomDatasetHeader::~DicomDatasetHeader()
{
}


void dcm::DicomDatasetHeader::clear()
{

  _binaryHeader.clear();
  _fileList.clear();

}


void dcm::DicomDatasetHeader::allocate( int32_t size )
{

  _binaryHeader.clear();
  _fileList.clear();
  _binaryHeader.resize( size );
  _fileList.resize( size, "" );

}


int32_t dcm::DicomDatasetHeader::size()
{

  return _binaryHeader.getCount();

}


bool dcm::DicomDatasetHeader::add( DcmDataset* dataset, 
                                   const std::string& fileName )
{

  if ( dataset )
  {

    dcm::BinaryHeader::Buffer buffer( 0, 0 );

    dataset->remove( DCM_PixelData );

    int32_t length = dataset->calcElementLength( EXS_LittleEndianExplicit,
                                                 EET_ExplicitLength );
    
    if ( _binaryHeader.allocate( buffer, length ) )
    {

      DcmOutputBufferStream dobs( buffer.first, buffer.second  );

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

      _binaryHeader.add( buffer );
      _fileList.push_back( fileName );

      return true;

    }

  }

  return false;

}


bool dcm::DicomDatasetHeader::add( int32_t i, 
                                   DcmDataset* dataset, 
                                   const std::string& fileName )
{

  if ( dataset )
  {

    dataset->remove( DCM_PixelData );

    int32_t length = dataset->calcElementLength( EXS_LittleEndianExplicit,
                                                 EET_ExplicitLength );
    
    if ( _binaryHeader.allocate( i, length ) )
    {

      DcmOutputBufferStream dobs( _binaryHeader[ i ].first,
                                  _binaryHeader[ i ].second  );

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

      _fileList[ i ] = fileName;

      return true;

    }

  }

  return false;

}


void dcm::DicomDatasetHeader::get( DcmDataset& dataset, int32_t i )
{

  if ( ( i >= 0 ) && ( i < _binaryHeader.getCount() ) )
  {

    if ( _binaryHeader[ i ].second )
    {

      DcmInputBufferStream dibs;

      dibs.setBuffer( (const void*)_binaryHeader[ i ].first,
                      _binaryHeader[ i ].second );
      dibs.setEos();
      dataset.clear();
      dataset.transferInit();
      dataset.read( dibs, EXS_LittleEndianExplicit );
      dataset.transferEnd();

    }

  }

}


void dcm::DicomDatasetHeader::trim()
{

  _binaryHeader.trim();

  std::vector< std::string >::iterator
    last = std::remove_if( _fileList.begin(),
                           _fileList.end(),
                           std::bind2nd( std::equal_to< std::string >(), "" ) );

  _fileList.erase( last, _fileList.end() );

}


bool dcm::DicomDatasetHeader::getDictionary( 
                              std::map< std::string, std::string >& dictionary )
{

  dictionary.clear();

  int32_t slice = 0;
  dcm::BinaryHeader::iterator
    d = _binaryHeader.begin(),
    de = _binaryHeader.end();

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


bool dcm::DicomDatasetHeader::setDictionary( 
                              std::map< std::string, std::string >& dictionary )
{

  if ( int32_t( dictionary.size() ) != _binaryHeader.getCount() )
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

    if ( _binaryHeader.allocate( slice, length ) )
    {

      std::istringstream issDataset( d->second );
      uint8_t* p = _binaryHeader[ slice ].first;
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


std::vector< int32_t >& dcm::DicomDatasetHeader::getLut()
{

  return _binaryHeader.getLut();

}


std::vector< std::string >& dcm::DicomDatasetHeader::getFileList()
{

  return _fileList;

}
