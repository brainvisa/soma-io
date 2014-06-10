#ifndef _DicomSortContext_h_
#define _DicomSortContext_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/SortInformation.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>
#include <map>


namespace soma
{


class DicomSortContext : public carto::LoopContext
{

  public:

    DicomSortContext( const std::string& seriesInstanceUID,
                      const std::vector< std::string >& files,
                      std::multimap< double, SortInformation >& slices,
                      const Vector& rowVector,
                      const Vector& columnVector,
                      int32_t& selectedFileCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::string m_seriesInstanceUID;
    const std::vector< std::string >& m_files;
    std::multimap< double, SortInformation >& m_slices;
    Vector m_absNormal;
    int32_t& m_selectedFileCount;

};


}


#endif
