#ifndef _Directory_h_
#define _Directory_h_


#include <soma-io/config/soma_config.h>

#include <string>
#include <vector>


namespace soma
{


class DirectoryParser
{

  public:

    DirectoryParser( const std::string& name );

    std::string& getSelectedFile();
    std::string& getSelectedDirectory();
    std::vector< std::string >& getFiles();

    bool parse();

  private :

    void parse( const std::string& dirName );

    std::string m_selectedFile;
    std::string m_selectedDir;
    std::vector< std::string > m_files;

};


}


#endif
