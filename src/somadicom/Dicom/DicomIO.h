#ifndef _DicomIO_h_
#define _DicomIO_h_


#include <Pattern/Singleton.h>

#include <string>


namespace soma
{


class Data;
class Callback;


class DicomIO : public Singleton< DicomIO >
{

  public:

    bool read( const std::string& fileName, 
               Data& data, 
               Callback* progress = 0 );

  protected:

    DicomIO();
    virtual ~DicomIO();

    friend class Singleton< DicomIO >;

};


}


#endif
