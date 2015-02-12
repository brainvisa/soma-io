#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/MultiFileReader.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Dicom/DicomDatasetHeader.h>
#include <soma-io/Container/DicomProxy.h>
#include <soma-io/Dicom/DicomSelectContext.h>
#include <soma-io/Dicom/DicomSortContext.h>
#include <soma-io/Dicom/DicomDataContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/MultiFileReader.h>
#include <System/DirectoryParser.h>
#include <Dicom/DicomDatasetHeader.h>
#include <Container/DicomProxy.h>
#include <Dicom/DicomSelectContext.h>
#include <Dicom/DicomSortContext.h>
#include <Dicom/DicomDataContext.h>
#include <Thread/ThreadedLoop.h>
#include <Utils/StdInt.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>


soma::MultiFileReader::MultiFileReader()
                     : soma::MultiSliceReader()
{
}


bool soma::MultiFileReader::readHeader( DcmDataset* dataset )
{

  if ( dataset )
  {

    OFString tmpString;

    if ( dataset->findAndGetOFStringArray( DCM_ImageOrientationPatient,
                                           tmpString ).good() )
    {

      std::string orientationStr = tmpString.c_str();
      size_t pos = orientationStr.find( "\\" );

      while ( pos != std::string::npos )
      {

        orientationStr[ pos ] = ' ';
        pos = orientationStr.find( "\\" );

      }

      std::istringstream iss( orientationStr );

      iss >> _dataInfo->_rowVec.x
          >> _dataInfo->_rowVec.y
          >> _dataInfo->_rowVec.z
          >> _dataInfo->_colVec.x
          >> _dataInfo->_colVec.y
          >> _dataInfo->_colVec.z;

    }

    return true;

  }

  return false;

}


bool soma::MultiFileReader::selectFiles( 
                                       soma::DirectoryParser& directory,
                                       const std::string& seriesInstanceUID,
                                       soma::DicomDatasetHeader& datasetHeader )
{

  int32_t n = int32_t( directory.getFiles().size() );

  if ( n )
  {

    int32_t fileCount = 0;

    datasetHeader.clear();
    datasetHeader.allocate( n );

    soma::DicomSelectContext context( directory,
                                      seriesInstanceUID,
                                      datasetHeader,
                                      fileCount );
    soma::ThreadedLoop threadedLoop( &context, 0, n );

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


bool soma::MultiFileReader::sortFiles( soma::DicomDatasetHeader& datasetHeader )
{

  int32_t n = datasetHeader.size();

  _positions.clear();

  if ( n )
  {

    std::multimap< double, soma::FileInformation > slices;

    soma::DicomSortContext context( datasetHeader,
                                    slices,
                                    _dataInfo->_rowVec,
                                    _dataInfo->_colVec );
    soma::ThreadedLoop threadedLoop( &context, 0, n );

    threadedLoop.launch();

    _positions.resize( n );

    _dataInfo->_frames = slices.count( slices.begin()->first );

    int32_t dimZ = n / _dataInfo->_frames;

    if ( !_dataInfo->_mosaic )
    {

      _dataInfo->_slices = dimZ;

    }

    std::multimap< double, soma::FileInformation >::iterator
      s = slices.begin(),
      se = slices.end();
    std::map< double, std::map< int32_t, soma::FileInformation > > ordered;

    while ( s != se )
    {

      std::map< double, std::map< int32_t, soma::FileInformation > >::iterator
        m = ordered.find( s->first );

      if ( m == ordered.end() )
      {

        std::map< int32_t, soma::FileInformation > orderedFiles;

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

    std::map< double, std::map< int32_t, soma::FileInformation > >::iterator
      o = ordered.begin(),
      oe = ordered.end();
    std::vector< int32_t >& lut = datasetHeader.getLut();
    int32_t origin = 0;

    while ( o != oe )
    {

      std::map< int32_t, soma::FileInformation >::iterator
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

    setOrientation();

    return true;

  }

  return false;

}


bool soma::MultiFileReader::readData( soma::DicomDatasetHeader& datasetHeader,
                                      soma::DicomProxy& proxy, 
                                      bool applyModalityLUT )
{

  if ( proxy.allocate() )
  {

    soma::DataInfo& info = proxy.getDataInfo();
    int32_t i, k = 0, n = int32_t( datasetHeader.size() );
    int32_t sizeX, sizeY, sizeZ;
    int32_t startZ = info._boundingBox.getLowerZ();
    int32_t endZ = info._boundingBox.getUpperZ() + 1;
    int32_t startT = info._boundingBox.getLowerT();
    int32_t endT = info._boundingBox.getUpperT() + 1;
    std::vector< int32_t > selection( ( endZ - startZ ) * ( endT - startT ) );

    info._patientOrientation.getOnDiskSize( sizeX, sizeY, sizeZ );

    for ( i = 0; i < n; i++ )
    {

      int32_t z = i % sizeZ;
      int32_t t = i / sizeZ;

      if ( ( z >= startZ ) && ( z < endZ ) && 
           ( t >= startT ) && ( t < endT ) )
      {

        selection[ k++ ] = i;

      }

    }

    soma::DicomDataContext context( datasetHeader, 
                                    proxy, 
                                    selection, 
                                    applyModalityLUT );
    soma::ThreadedLoop threadedLoop( &context, 0, int32_t( selection.size() ) );

    threadedLoop.launch();

    return true;

  }

  return false;

}

