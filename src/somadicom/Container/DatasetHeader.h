#ifndef _DatasetHeader_h_
#define _DatasetHeader_h_


#include <soma-io/config/soma_config.h>

#include <soma-io/Utils/StdInt.h>


namespace soma
{


class DatasetHeader
{

  public:

    DatasetHeader( uint32_t length = 0 );
    virtual ~DatasetHeader();

    bool allocate( uint32_t length );

    uint8_t* getBuffer();
    uint32_t getLength();

  protected:

    uint8_t* m_buffer;
    uint32_t m_length;

};


}


#endif
