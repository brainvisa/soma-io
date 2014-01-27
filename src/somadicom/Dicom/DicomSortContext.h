#ifndef _DicomSortContext_h_
#define _DicomSortContext_h_


#include <Dicom/SortInformation.h>
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>

#include <string>
#include <vector>
#include <map>


namespace soma
{


class DicomSortContext : public LoopContext
{

  public:

    DicomSortContext( const std::string& seriesInstanceUID,
                      const std::vector< std::string >& files,
                      std::multimap< double, SortInformation >& slices,
                      int32_t& selectedFileCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const std::string m_seriesInstanceUID;
    const std::vector< std::string >& m_files;
    std::multimap< double, SortInformation >& m_slices;
    int32_t& m_selectedFileCount;

};


}


#endif
