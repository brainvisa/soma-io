#ifndef _SortInformation_h_
#define _SortInformation_h_


#include <Container/Vector.h>
#include <Utils/StdInt.h>

#include <string>


namespace soma
{


class SortInformation
{

  public:

    SortInformation();

    int32_t m_instanceNumber;
    double m_echoTime;
    Vector m_imagePosition;
    std::string m_fileName;

};


}

#endif
