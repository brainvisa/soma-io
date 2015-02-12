#ifndef _DirectoryParser_h_
#define _DirectoryParser_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#endif

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

  private :

    std::string getFirstFile();
    void parse();

    std::string _selectedFile;
    std::string _selectedDir;
    std::set< std::string > _files;

};


}


#endif
