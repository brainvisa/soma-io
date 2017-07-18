#ifndef _MRModule_h_
#define _MRModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


class DcmDataset;


namespace dcm
{


class MRModule : public DicomModule
{

  public:

    MRModule();

    bool parseItem( DcmItem* dcmItem );
    bool parseDataset( DcmDataset* dadaset );
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    const std::vector< double >& getTR() const;
    const std::vector< double >& getTE() const;
    const std::vector< double >& getTI() const;
    double getFlipAngle() const;

  protected:

    virtual int32_t getStep( DicomDatasetHeader& datasetHeader );

  private:

    std::vector< double > _tr;
    std::vector< double > _te;
    std::vector< double > _ti;
    double _flipAngle;

}; 


}


#endif
