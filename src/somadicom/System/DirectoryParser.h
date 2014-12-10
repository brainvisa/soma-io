#ifndef _DirectoryParser_h_
#define _DirectoryParser_h_


#include <soma-io/config/soma_config.h>

#include <string>
#include <set>


namespace soma
{


class DirectoryParser
{

  public:

    DirectoryParser( const std::string& name );

    std::string& getSelectedFile();
    std::string& getSelectedDirectory();
    std::set< std::string >& getFiles();

    bool parse();

  private :

    void parse( const std::string& dirName );

    std::string m_selectedFile;
    std::string m_selectedDir;
    std::set< std::string > m_files;

};


}


#endif
