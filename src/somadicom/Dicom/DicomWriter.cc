#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DicomWriter.h>
#include <soma-io/Container/DicomProxy.h>
#else
#include <Dicom/DicomWriter.h>
#include <Container/DicomProxy.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcxfer.h>
#include <dcmtk/dcmdata/dcuid.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef MINI_VIEWER
#include <sys/time.h>
#include <iostream>
#endif


dcm::DicomWriter::DicomWriter()
{
}


dcm::DicomWriter::~DicomWriter()
{
}


bool dcm::DicomWriter::write( const std::string& fileName,
                              dcm::DicomProxy& proxy )
{

  if ( !fileName.empty() )
  {

    std::string baseDirectory = "./";
    std::string outputFileName = "Unknown";
    std::string extension = "";

    size_t pos = fileName.rfind( ".dcm" );
    if ( pos == std::string::npos )
    {

      pos = fileName.rfind( ".dic" );

    }

    if ( pos != std::string::npos )
    {

      outputFileName = fileName.substr( 0, pos );
      extension = fileName.substr( pos );

      pos = outputFileName.rfind( "/" );

      if ( pos != std::string::npos )
      {

        baseDirectory = outputFileName.substr( 0, pos + 1 );
        outputFileName = outputFileName.substr( pos + 1 );

      }

    }
    else
    {

      if ( fileName[ fileName.length() - 1 ] == '/' )
      {

        baseDirectory = fileName;

      }
      else
      {

        outputFileName = fileName;
        pos = outputFileName.rfind( "/" );

        if ( pos != std::string::npos )
        {

          baseDirectory = outputFileName.substr( 0, pos + 1 );
          outputFileName = outputFileName.substr( pos + 1 );

        }

      }

    }

    struct stat file_stat;

    if ( stat( baseDirectory.c_str(), &file_stat ) )
    {

      return false;

    }

    if ( !S_ISDIR( file_stat.st_mode ) )
    {

      return false;

    }

    if ( baseDirectory[ baseDirectory.length() - 1 ] != '/' )
    {

      baseDirectory += "/";

    }

#ifdef MINI_VIEWER
    struct timeval tv_start;
    struct timeval tv_stop;

    gettimeofday( &tv_start, NULL );
#endif

    bool status = writeDatasets( baseDirectory, 
                                 outputFileName + extension, 
                                 proxy );

#ifdef MINI_VIEWER
    gettimeofday( &tv_stop, NULL );

    double dt = (double)( tv_stop.tv_sec + tv_stop.tv_usec / 1000000.0 - 
                          tv_start.tv_sec - tv_start.tv_usec / 1000000.0 );
    std::cout << "Write time : " << dt << " s" << std::endl;
#endif

    return status;

  }

  return false;

}
