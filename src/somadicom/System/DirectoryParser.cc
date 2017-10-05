#ifdef SOMA_IO_DICOM
#include <soma-io/System/DirectoryParser.h>
#else
#include <System/DirectoryParser.h>
#endif

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>


#ifdef WIN32
#define DICOM_PATHSEP "\\"
#define DICOM_ALTPATHSEP "/"
#else
#define DICOM_PATHSEP "/"
#endif

dcm::DirectoryParser::DirectoryParser( const std::string& name )
                    : _selectedFile( "" ),
                      _selectedDir( "" )
{

  struct stat file_stat;

  if ( !stat( name.c_str(), &file_stat ) )
  {
    if ( S_ISDIR( file_stat.st_mode ) )
    {

      _selectedDir = name;
      _selectedFile = getFirstFile();

    }
    else if ( S_ISREG( file_stat.st_mode ) )
    {

      _selectedFile = name;
      _selectedDir = ".";

      size_t pos = name.find_last_of( DICOM_PATHSEP );
#if defined( DICOM_ALTPATHSEP )
      size_t pos2 = name.find_last_of( DICOM_ALTPATHSEP );
      if ((pos2 != std::string::npos) 
          && (pos == std::string::npos || pos2 > pos))
        pos = pos2;
#endif

      if ( pos != std::string::npos )
      {

        _selectedDir = name.substr( 0, pos );

      }

    }
    else
    {
      _selectedDir = ".";

      size_t pos = name.find_last_of( DICOM_PATHSEP );
#if defined( DICOM_ALTPATHSEP )
      size_t pos2 = name.find_last_of(DICOM_ALTPATHSEP);
      if ((pos2 != std::string::npos) 
          && (pos == std::string::npos || pos2 > pos))
        pos = pos2;
#endif
      if ( pos != std::string::npos )
      {

        _selectedDir = name.substr( 0, pos );

      }

      _selectedFile = getFirstFile();

    }

  }
}


std::string dcm::DirectoryParser::getFirstFile()
{

  std::string fileName = "";

  if ( !_selectedDir.empty() )
  {

    DIR* directory = opendir( _selectedDir.c_str() );

    if ( directory )
    {

      bool found = false;
      struct dirent* item;
      struct stat file_stat;

      while ( !found && ( item = readdir( directory ) ) )
      {

        std::ostringstream fname;

        fname << _selectedDir << DICOM_PATHSEP << item->d_name;
        stat( fname.str().c_str(), &file_stat );

        if ( S_ISREG( file_stat.st_mode ) )
        {

          fileName = fname.str();
          found = true;

        }

      }

      closedir( directory );

    }

  }

  return fileName;

}


void dcm::DirectoryParser::parse()
{

  if ( !_selectedDir.empty() )
  {

    DIR* directory = opendir( _selectedDir.c_str() );

    if ( directory )
    {

      struct dirent* item;
      struct stat file_stat;

      while ( ( item = readdir( directory ) ) )
      {

        std::ostringstream fname;

        fname << _selectedDir << DICOM_PATHSEP << item->d_name;
        stat( fname.str().c_str(), &file_stat );

        if ( S_ISREG( file_stat.st_mode ) )
        {

          _files.insert( fname.str() );

        }

      }

      closedir( directory );

    }

  }

}


std::string& dcm::DirectoryParser::getSelectedFile()
{

  return _selectedFile;

}


std::string& dcm::DirectoryParser::getSelectedDirectory()
{

  return _selectedDir;

}


std::set< std::string >& dcm::DirectoryParser::getFiles()
{

  if ( _files.empty() )
  {

    parse();

  }

  return _files;

}
