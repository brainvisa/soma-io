#ifndef _DicomSortContext_h_
#define _DicomSortContext_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/FileInformation.h>
#include <cartobase/thread/loopContext.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/FileInformation.h>
#include <Thread/LoopContext.h>
#include <Utils/StdInt.h>
#endif

#include <map>


namespace soma
{


class DicomDatasetHeader;


class DicomSortContext : public LoopContext
{

  public:

    DicomSortContext( DicomDatasetHeader& datasetHeader,
                      std::multimap< double, FileInformation >& slices,
                      const Vector& rowVector,
                      const Vector& columnVector );

    void doIt( int32_t startIndex, int32_t count );

  private:

    DicomDatasetHeader& _datasetHeader;
    std::multimap< double, FileInformation >& _slices;
    Vector _normal;

};


}


#endif
