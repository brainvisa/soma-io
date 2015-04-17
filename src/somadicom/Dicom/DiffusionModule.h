#ifndef _DiffusionModule_h_
#define _DiffusionModule_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomModule.h>
#include <soma-io/Transformation/DicomTransform3d.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Dicom/DicomModule.h>
#include <Transformation/DicomTransform3d.h>
#include <Utils/StdInt.h>
#endif

#include <vector>


namespace dcm
{


class DiffusionModule : public DicomModule
{

  public:

    DiffusionModule();

    virtual bool parseItem( DcmItem* dcmItem );
    
    bool parseHeader( DicomDatasetHeader& datasetHeader );

    const std::vector< double >& getBValues() const;
    const std::vector< std::vector< double > >& getDirections() const;

  protected:

    void addDirection( std::vector< double >& direction );

    virtual int32_t getStep( DicomDatasetHeader& datasetHeader );

    DicomTransform3d< double > _dicomTransform;
    std::vector< double > _bValues;
    std::vector< std::vector< double > > _directions;

};


}


#endif
