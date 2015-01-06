#include <soma-io/Dicom/MultiFileReader.h>
#include <soma-io/System/DirectoryParser.h>
#include <soma-io/Container/DataInfo.h>
#include <soma-io/Dicom/DicomSortContext.h>
#include <cartobase/thread/threadedLoop.h>
#include <soma-io/Pattern/Callback.h>
#include <soma-io/Utils/StdInt.h>

#include <soma-io/Dicom/soma_osconfig.h>
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

      iss >> m_dataInfo->m_rowVec.x
          >> m_dataInfo->m_rowVec.y
          >> m_dataInfo->m_rowVec.z
          >> m_dataInfo->m_colVec.x
          >> m_dataInfo->m_colVec.y
          >> m_dataInfo->m_colVec.z;

    }

    return true;

  }

  return false;

}


std::vector< std::string > soma::MultiFileReader::sortFiles( 
                                              soma::DirectoryParser& directory )
{

  bool process = true;

  m_slices.clear();
  m_positions.clear();
  m_dataInfo->m_fileCount = 0;

  if ( directory.getFiles().empty() )
  {

    process = directory.parse();

  }

  if ( process )
  {

    int32_t nFiles = int32_t( directory.getFiles().size() );

    std::multimap< double, soma::FileInformation > slices;
    soma::DicomSortContext context( m_seriesInstanceUID, 
                                    directory.getFiles(),
                                    slices,
                                    m_dataInfo->m_rowVec,
                                    m_dataInfo->m_colVec,
                                    m_dataInfo->m_fileCount );
    carto::ThreadedLoop threadedLoop( &context, 0, nFiles );

    threadedLoop.launch();

    m_slices.resize( m_dataInfo->m_fileCount );
    m_positions.resize( m_dataInfo->m_fileCount );
    m_dataInfo->m_slope.resize( m_dataInfo->m_fileCount, 1.0 );
    m_dataInfo->m_intercept.resize( m_dataInfo->m_fileCount, 0.0 );

    m_dataInfo->m_frames = slices.count( slices.begin()->first );

    int32_t dimZ = m_dataInfo->m_fileCount / m_dataInfo->m_frames;

    if ( !m_dataInfo->m_mosaic )
    {

      m_dataInfo->m_slices = dimZ;

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

        orderedFiles.insert( std::make_pair( s->second.m_instanceNumber,
                                             s->second ) );
        ordered.insert( std::make_pair( s->first, orderedFiles ) );

      }
      else
      {

        m->second.insert( std::make_pair( s->second.m_instanceNumber,
                                          s->second ) );

      }

      ++s;

    }

    std::map< double, std::map< int32_t, soma::FileInformation > >::iterator
      o = ordered.begin(),
      oe = ordered.end();
    int32_t origin = 0;

    while ( o != oe )
    {

      std::map< int32_t, soma::FileInformation >::iterator
        f = o->second.begin(),
        fe = o->second.end();
      int32_t pos = origin;

      while ( f != fe )
      {

        m_slices[ pos ] = f->second.m_fileName;
        m_positions[ pos ] = f->second.m_imagePosition;
        m_dataInfo->m_slope[ pos ] = f->second.m_slope;
        m_dataInfo->m_intercept[ pos ] = f->second.m_intercept;
        
        pos += dimZ;
        ++f;

      }

      origin++;
      ++o;

    }

    setOrientation();

  }

  return m_slices;

}
