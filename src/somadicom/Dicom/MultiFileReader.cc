#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFileReader.h>
#include <soma-io/Dicom/OrientationModule.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Object/HeaderProxy.h>
#include <soma-io/Dicom/DicomSelectContext.h>
#include <soma-io/Dicom/DicomSortContext.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/MultiFileReader.h>
#include <Dicom/OrientationModule.h>
#include <System/DirectoryParser.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Object/HeaderProxy.h>
#include <Dicom/DicomSelectContext.h>
#include <Dicom/DicomSortContext.h>
#include <Dicom/DicomDataContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::MultiFileReader::MultiFileReader()
                    : dcm::DicomReader(),
                      dcm::MultiSliceReader()
{
}


bool dcm::MultiFileReader::getHeader( dcm::HeaderProxy& header, 
                                      dcm::DataInfo& dataInfo,
                                      dcm::DicomDatasetHeader& datasetHeader )
{

  if ( dcm::MultiSliceReader::getHeader( header, dataInfo, datasetHeader ) )
  {

    return dcm::DicomReader::getHeader( header, dataInfo, datasetHeader );

  }

  return false;

}


bool dcm::MultiFileReader::readHeader( DcmDataset* dataset )
{

  dcm::OrientationModule orientationModule;

  if ( orientationModule.parseItem( dataset ) )
  {

    _dataInfo->_rowVec = orientationModule.getRowCosine();
    _dataInfo->_colVec = orientationModule.getColumnCosine();

    return true;

  }

  return false;

}


bool dcm::MultiFileReader::selectFiles( dcm::DirectoryParser& directory,
                                        const std::string& seriesInstanceUID,
                                        dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t n = int32_t( directory.getFiles().size() );

  if ( n )
  {

    int32_t fileCount = 0;

    datasetHeader.clear();
    datasetHeader.allocate( n );

    dcm::DicomSelectContext context( directory,
                                     seriesInstanceUID,
                                     datasetHeader,
                                     fileCount );
#ifdef SOMA_IO_DICOM
    carto::ThreadedLoop threadedLoop( &context, 0, n );
#else
    dcm::ThreadedLoop threadedLoop( &context, 0, n );
#endif

    threadedLoop.launch();

    if ( fileCount < n )
    {

      datasetHeader.trim();

    }

    if ( fileCount )
    {

      _dataInfo->_fileCount = fileCount;

      return true;

    }

  }

  return false;

}


bool dcm::MultiFileReader::sortFiles( dcm::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  _positions.clear();

  if ( n )
  {

    std::multimap< double, dcm::FileInformation > slices;

    dcm::DicomSortContext context( datasetHeader,
                                   slices,
                                   _dataInfo->_rowVec,
                                   _dataInfo->_colVec );
#ifdef SOMA_IO_DICOM
    carto::ThreadedLoop threadedLoop( &context, 0, n );
#else
    dcm::ThreadedLoop threadedLoop( &context, 0, n );
#endif

    threadedLoop.launch();

    _positions.resize( n );

    _dataInfo->_frames = slices.count( slices.begin()->first );

    int32_t dimZ = n / _dataInfo->_frames;

    if ( !_dataInfo->_mosaic || _dataInfo->_noDemosaic )
    {

      _dataInfo->_slices = dimZ;

    }

    std::multimap< double, dcm::FileInformation >::iterator
      s = slices.begin(),
      se = slices.end();
    std::map< double, std::map< int32_t, dcm::FileInformation > > ordered;

    while ( s != se )
    {

      std::map< double, std::map< int32_t, dcm::FileInformation > >::iterator
        m = ordered.find( s->first );

      if ( m == ordered.end() )
      {

        std::map< int32_t, dcm::FileInformation > orderedFiles;

        orderedFiles.insert( std::make_pair( s->second._instanceNumber,
                                             s->second ) );
        ordered.insert( std::make_pair( s->first, orderedFiles ) );

      }
      else
      {

        m->second.insert( std::make_pair( s->second._instanceNumber,
                                          s->second ) );

      }

      ++s;

    }

    std::map< double, std::map< int32_t, dcm::FileInformation > >::iterator
      o = ordered.begin(),
      oe = ordered.end();
    std::vector< int32_t >& lut = datasetHeader.getLut();
    int32_t origin = 0;

    while ( o != oe )
    {

      std::map< int32_t, dcm::FileInformation >::iterator
        f = o->second.begin(),
        fe = o->second.end();
      int32_t pos = origin;

      while ( f != fe )
      {

        lut[ pos ] = f->second._bufferIndex;
        _positions[ pos ] = f->second._imagePosition;
        
        pos += dimZ;
        ++f;

      }

      origin++;
      ++o;

    }

    dcm::MultiSliceReader::setOrientation( *_dataInfo );
    dcm::DicomReader::setOrientation();

    return true;

  }

  return false;

}


bool dcm::MultiFileReader::readData( dcm::DicomDatasetHeader& datasetHeader,
                                     dcm::DicomProxy& proxy )
{

  if ( proxy.allocate() )
  {

    dcm::DataInfo& info = proxy.getDataInfo();
    int32_t i, k = 0, n = int32_t( datasetHeader.size() );
    int32_t startZ = info._boundingBox.getLowerZ();
    int32_t endZ = info._boundingBox.getUpperZ() + 1;
    int32_t startT = info._boundingBox.getLowerT();
    int32_t endT = info._boundingBox.getUpperT() + 1;
    std::vector< int32_t > selection( ( endZ - startZ ) * ( endT - startT ) );
    dcm::Vector3d< int32_t > sizes = info._patientOrientation.getOnDiskSize();

    for ( i = 0; i < n; i++ )
    {

      int32_t z = i % sizes.z;
      int32_t t = i / sizes.z;

      if ( ( z >= startZ ) && ( z < endZ ) && 
           ( t >= startT ) && ( t < endT ) )
      {

        selection[ k++ ] = i;

      }

    }

    dcm::DicomDataContext context( datasetHeader, proxy, selection );
#ifdef SOMA_IO_DICOM
    carto::ThreadedLoop threadedLoop( &context,
                                      0, 
                                      int32_t( selection.size() ) );
#else
    dcm::ThreadedLoop threadedLoop( &context, 0, int32_t( selection.size() ) );
#endif

    threadedLoop.launch();

    if ( ( info._bpp < 3 ) && ( info._minimum >= info._maximum ) )
    {

      info._minimum = 0;
      info._maximum = ( 1 << info._bitsStored ) - 1;

    }

    return true;

  }

  return false;

}
