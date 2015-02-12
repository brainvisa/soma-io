#ifndef _DicomSelectContext_h_
#define _DicomSelectContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <string>
#include <vector>


namespace soma
{


class DirectoryParser;
class DicomDatasetHeader;


class DicomSelectContext : public LoopContext
{

  public:

    DicomSelectContext( DirectoryParser& directory,
                        const std::string& seriesInstanceUID,
                        DicomDatasetHeader& datasetHeader,
                        int32_t& fileCount );

    void doIt( int32_t startIndex, int32_t count );

  private:

    std::vector< std::string > _files;
    const std::string& _seriesInstanceUID;
    DicomDatasetHeader& _datasetHeader;
    int32_t& _fileCount;

};


}


#endif
