#include <soma-io/Dicom/FileInformation.h>


soma::FileInformation::FileInformation()
                     : m_instanceNumber( 0 ),
                       m_echoTime( 0.0 ),
                       m_slope( 1.0 ),
                       m_intercept( 0.0 ),
                       m_fileName( "" )
{
}
