#ifndef _FileInformation_h_
#define _FileInformation_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Container/Vector.h>
#include <soma-io/Utils/StdInt.h>

#include <string>


namespace soma
{


class FileInformation
{

  public:

    FileInformation();

    int32_t m_instanceNumber;
    double m_echoTime;
    double m_slope;
    double m_intercept;
    Vector m_imagePosition;
    std::string m_fileName;

};


}

#endif
