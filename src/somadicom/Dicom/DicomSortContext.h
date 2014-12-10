#ifndef _DicomSortContext_h_
#define _DicomSortContext_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/FileInformation.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <set>
#include <vector>
#include <map>


namespace soma
{


class DicomSortContext : public carto::LoopContext
{

  public:

    DicomSortContext( const std::string& seriesInstanceUID,
                      const std::set< std::string >& files,
                      std::multimap< double, FileInformation >& slices,
                      const Vector& rowVector,
                      const Vector& columnVector,
                      int32_t& selectedFileCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::string m_seriesInstanceUID;
    std::vector< std::string > m_files;
    std::multimap< double, FileInformation >& m_slices;
    Vector m_absNormal;
    int32_t& m_selectedFileCount;

};


}


#endif
