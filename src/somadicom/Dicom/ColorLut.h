#ifndef _ColorLut_h_
#define _ColorLut_h_


#ifdef SOMA_IO_DICOM
#include <soma-io/config/soma_config.h>
#include <soma-io/Utils/StdInt.h>
#else
#include <Utils/StdInt.h>
#endif


class DcmItem;
class DcmTagKey;


namespace dcm
{


class ColorLut
{

  public:

    ColorLut();
    virtual ~ColorLut();

    void initialize( DcmItem* item, 
                     const DcmTagKey& descriptor, 
                     const DcmTagKey& data );

    int32_t getBits() const;

    uint16_t operator()( int32_t index );

  private:

    uint32_t _count;
    int32_t _first;
    int32_t _bits;
    uint16_t* _lut;

};


}


#endif
