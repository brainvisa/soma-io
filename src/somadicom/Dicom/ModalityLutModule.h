#ifndef _ModalityLutModule_h_
#define _ModalityLutModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#else
#include <Dicom/DicomModule.h>
#endif

#include <vector>


class DcmDataset;


namespace dcm
{


class ModalityLutModule : public DicomModule
{

  public:

    ModalityLutModule();

    virtual bool parseItem( DcmItem* dcmItem );
    virtual bool parseDataset( DcmDataset* dadaset );
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    const std::vector< double >& getRescaleIntercept() const;
    const std::vector< double >& getRescaleSlope() const;

  protected:

    std::vector< double > _rescaleIntercept;
    std::vector< double > _rescaleSlope;

};


}


#endif
