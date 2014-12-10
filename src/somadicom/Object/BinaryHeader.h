#ifndef _BinaryHeader_h_
#define _BinaryHeader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>

#include <string>
#include <vector>
#include <map>


namespace soma
{


class BinaryHeader
{

  public:

    typedef std::pair< uint8_t*, uint32_t > Buffer;
    typedef std::vector< Buffer >::iterator iterator;
    typedef std::vector< Buffer >::const_iterator const_iterator;

    BinaryHeader();
    BinaryHeader( const BinaryHeader& other );
    virtual ~BinaryHeader();

    BinaryHeader& operator = ( const BinaryHeader& other );
    Buffer& operator [] ( uint32_t index );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    int32_t getCount() const;

    void resize( uint32_t n, Buffer buffer = Buffer( 0, 0 ) );
    bool allocate( uint32_t index, uint32_t length );
    void clear();

    void setFormat( const std::string& format );
    const std::string& getFormat() const;

  protected:

    std::vector< Buffer > m_buffers;
    std::string m_format;

};


}


#endif
