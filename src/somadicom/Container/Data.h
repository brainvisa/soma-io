#ifndef _Data_h_
#define _Data_h_


#include <Container/DataInfo.h>

#include <vector>


namespace soma
{


class Data
{

  public:

    Data();
    ~Data();

    bool Create( const DataInfo& info );
    void Destroy();
    bool IsNull();

    uint8_t* getLine( int32_t y, int32_t z, int32_t t = 0 );
    uint8_t* getSlice( int32_t z, int32_t t = 0 );
    uint8_t* getFrame( int32_t t );

    DataInfo m_info;

    uint8_t* m_data;

    std::vector< std::vector< std::vector< uint8_t* > > > m_lineAccess;

};


}


#endif
