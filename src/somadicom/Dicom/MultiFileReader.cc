#include <soma-io/Dicom/MultiFileReader.h>
#include <soma-io/System/Directory.h>
#include <soma-io/Container/Data.h>
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


bool soma::MultiFileReader::read( Directory& directory, 
                                  soma::Data& data, 
                                  soma::Callback* progress )
{

  initialize();

  if ( progress )
  {

    progress->execute( 0 );

  }

  std::string selectedFile = directory.getSelectedFile();

  if ( !selectedFile.empty() )
  {

    if ( progress )
    {

      progress->execute( 2 );

    }

    // read global information from selected image
    if ( soma::DicomReader::readHeader( selectedFile ) )
    {

      if ( progress )
      {

        progress->execute( 4 );

      }

      // order files according to location and repetition
      if ( sortFiles( directory ) )
      {

        if ( progress )
        {

          progress->execute( 6 );

        }

        // read all slices in the serie
        bool status = readData( data, progress );

        if ( progress )
        {

          progress->execute( 100 );

        }

        return status;

      }

    }

  }

  return false;

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

      iss >> m_dataInfo.m_rowVec.x
          >> m_dataInfo.m_rowVec.y
          >> m_dataInfo.m_rowVec.z
          >> m_dataInfo.m_colVec.x
          >> m_dataInfo.m_colVec.y
          >> m_dataInfo.m_colVec.z;

    }

    return true;

  }

  return false;

}


bool soma::MultiFileReader::sortFiles( soma::Directory& directory )
{

  bool process = true;

  if ( directory.getFiles().empty() )
  {

    process = directory.parse();

  }

  if ( process )
  {

    int32_t nFiles = int32_t( directory.getFiles().size() );

    m_slices.clear();
    m_positions.clear();

    std::multimap< double, soma::SortInformation > slices;
    int32_t selectedFileCount = 0;
    soma::DicomSortContext context( m_seriesInstanceUID, 
                                    directory.getFiles(),
                                    slices,
                                    selectedFileCount );
    carto::ThreadedLoop threadedLoop( &context,
                                     0,
                                     nFiles );

    threadedLoop.launch();
    m_slices.resize( selectedFileCount );
    m_positions.resize( selectedFileCount );

    m_dataInfo.m_frames = slices.count( slices.begin()->first );
    m_dataInfo.m_slices = selectedFileCount / m_dataInfo.m_frames;

    std::multimap< double, soma::SortInformation >::iterator
      s = slices.begin(),
      se = slices.end();
    std::map< double, std::map< int32_t, soma::SortInformation > > ordered;

    while ( s != se )
    {

      std::map< double, std::map< int32_t, soma::SortInformation > >::iterator
        m = ordered.find( s->first );

      if ( m == ordered.end() )
      {

        std::map< int32_t, soma::SortInformation > orderedFiles;

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

    std::map< double, std::map< int32_t, soma::SortInformation > >::iterator
      o = ordered.begin(),
      oe = ordered.end();
    int32_t origin = 0;
    int32_t dimZ = m_dataInfo.m_slices;

    while ( o != oe )
    {

      std::map< int32_t, soma::SortInformation >::iterator
        f = o->second.begin(),
        fe = o->second.end();
      int32_t pos = origin;

      while ( f != fe )
      {

        m_slices[ pos ] = f->second.m_fileName;
        m_positions[ pos ] = f->second.m_imagePosition;
        
        pos += dimZ;
        ++f;

      }

      origin++;
      ++o;

    }

    return true;

  }

  return false;

}
