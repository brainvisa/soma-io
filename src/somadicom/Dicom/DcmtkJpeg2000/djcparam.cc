#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#endif

#include <dcmtk/ofstd/ofstd.h>


DJ2KCodecParameter::DJ2KCodecParameter( Uint32 fragmentSize,
                                        OFBool createOffsetTable,
                                        J2K_UIDCreation uidCreation,
                                        OFBool convertToSC )
                  : DcmCodecParameter(),
                    fragmentSize_( fragmentSize ),
                    createOffsetTable_( createOffsetTable ),
                    uidCreation_( uidCreation ),
                    convertToSC_( convertToSC )
{
}


DJ2KCodecParameter::DJ2KCodecParameter( const DJ2KCodecParameter& arg )
                  : DcmCodecParameter( arg ),
                    fragmentSize_( arg.fragmentSize_ ),
                    createOffsetTable_( arg.createOffsetTable_ ),
                    uidCreation_( arg.uidCreation_ ),
                    convertToSC_( arg.convertToSC_ )
{
}


DJ2KCodecParameter::~DJ2KCodecParameter()
{
}


DcmCodecParameter* DJ2KCodecParameter::clone() const
{

  return new DJ2KCodecParameter( *this );

}


const char* DJ2KCodecParameter::className() const
{

  return "DJ2KCodecParameter";

}


Uint32 DJ2KCodecParameter::getFragmentSize() const
{

  return fragmentSize_;

}


OFBool DJ2KCodecParameter::getCreateOffsetTable() const
{

  return createOffsetTable_;

}


J2K_UIDCreation DJ2KCodecParameter::getUIDCreation() const
{

  return uidCreation_;

}


OFBool DJ2KCodecParameter::getConvertToSC() const
{

  return convertToSC_;

}
