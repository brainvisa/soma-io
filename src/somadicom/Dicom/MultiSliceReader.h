#ifndef _MultiSliceReader_h_
#define _MultiSliceReader_h_


#include <Dicom/DicomReader.h>


namespace soma
{


class MultiSliceReader : public DicomReader
{

  public:

    MultiSliceReader();

    void setOrientation( DataInfo& dataInfo );

  protected:

    std::vector< Vector > m_positions;
    Vector m_origin;

};


}


#endif
