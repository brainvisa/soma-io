#ifndef _MultiSliceReader_h_
#define _MultiSliceReader_h_


#include <soma-io/config/soma_config.h>
#include <soma-io/Dicom/DicomReader.h>


namespace soma
{


class MultiSliceReader : public DicomReader
{

  public:

    MultiSliceReader();

    void setOrientation();

  protected:

    std::vector< Vector > m_positions;
    Vector m_origin;

};


}


#endif
