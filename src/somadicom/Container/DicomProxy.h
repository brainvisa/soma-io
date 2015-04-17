#ifndef _DicomProxy_h_
#define _DicomProxy_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Container/DataInfo.h>
#else
#include <Pattern/Model.h>
#include <Container/DataInfo.h>
#endif

#include <vector>


namespace dcm
{


#ifdef SOMA_IO_DICOM
class DicomProxy
#else
class DicomProxy : public Model
#endif
{

  public:

    DicomProxy( DataInfo* info = 0 );
    DicomProxy( uint8_t* ptr, DataInfo* info = 0 );
    ~DicomProxy();

    virtual bool allocate( DataInfo* info = 0 );
    virtual void destroy();
    virtual bool isNull() const;
    virtual bool isMemoryMapped() const;

    DataInfo& getDataInfo();

    virtual BinaryHeader& getBinaryHeader();

    virtual uint8_t* operator()( int32_t x,
                                 int32_t y = 0, 
                                 int32_t z = 0, 
                                 int32_t t = 0 );

  protected:

    uint8_t* _buffer;
    bool _dataOwner;
    DataInfo _info;

    std::vector< std::vector< std::vector< uint8_t* > > > _lineAccess;

};


}


#endif
