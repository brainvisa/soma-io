#ifndef _DicomProxy_h_
#define _DicomProxy_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Container/DataInfo.h>

#include <vector>


namespace soma
{


class DicomProxy
{

  public:

    DicomProxy( DataInfo* info = 0 );
    DicomProxy( uint8_t* ptr, DataInfo* info = 0  );
    ~DicomProxy();

    virtual bool allocate( DataInfo* info = 0 );
    virtual void destroy();
    virtual bool isNull();
    virtual bool isMemoryMapped() const;

    DataInfo& getDataInfo();

    virtual BinaryHeader& getBinaryHeader();

    virtual uint8_t* operator() ( int32_t x,
                                  int32_t y = 0, 
                                  int32_t z = 0, 
                                  int32_t t = 0 );

  protected:

    uint8_t* m_buffer;
    bool m_dataOwner;
    DataInfo m_info;

    std::vector< std::vector< std::vector< uint8_t* > > > m_lineAccess;

};


}


#endif
