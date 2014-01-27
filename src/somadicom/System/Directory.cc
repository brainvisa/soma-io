#include <System/Directory.h>

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>


soma::Directory::Directory( const std::string& name )
               : m_selectedFile( "" ),
                 m_selectedDir( "" )
{

  struct stat file_stat;

  if ( !stat( name.c_str(), &file_stat ) )
  {

    if ( S_ISDIR( file_stat.st_mode ) )
    {

      m_selectedDir = name;
      parse( name );

    }
    else if ( S_ISREG( file_stat.st_mode ) )
    {

      m_selectedFile = name;
      m_selectedDir = ".";

      size_t pos = name.find_last_of( "/" );

      if ( pos != std::string::npos )
      {

        m_selectedDir = name.substr( 0, pos );

      }

    }

  }

}


bool soma::Directory::parse()
{

  if ( !m_selectedDir.empty() )
  {

    m_files.clear();

    parse( m_selectedDir );

    return true;

  }

  return false;

}


void soma::Directory::parse( const std::string& dirName )
{

  if ( !dirName.empty() )
  {

    DIR* directory = opendir( dirName.c_str() );

    if ( directory )
    {

      struct dirent* item;
      struct stat file_stat;

      while ( ( item = readdir( directory ) ) )
      {

        std::ostringstream fname;

        fname << dirName << "/" << item->d_name;
        stat( fname.str().c_str(), &file_stat );

        if ( S_ISREG( file_stat.st_mode ) )
        {

          if ( m_selectedFile.empty() )
          {

            m_selectedFile = fname.str();

          }

          m_files.push_back( fname.str() );

        }

      }

    }

  }

}


std::string& soma::Directory::getSelectedFile()
{

  return m_selectedFile;

}


std::string& soma::Directory::getSelectedDirectory()
{

  return m_selectedDir;

}


std::vector< std::string >& soma::Directory::getFiles()
{

  return m_files;

}
