#include <dcmtk/config/osconfig.h>

#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/DcmtkJpeg2000/djcparam.h>
#else
#include <Dicom/DcmtkJpeg2000/djcparam.h>
#endif

#include <dcmtk/ofstd/ofstd.h>


DJ2KCodecParameter::DJ2KCodecParameter( J2K_UIDCreation uidCreation,
                                        OFBool convertToSC )
                  : DcmCodecParameter(),
                    uidCreation_( uidCreation ),
                    convertToSC_( convertToSC )
{
}


DJ2KCodecParameter::DJ2KCodecParameter( const DJ2KCodecParameter& arg )
                  : DcmCodecParameter( arg ),
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


J2K_UIDCreation DJ2KCodecParameter::getUIDCreation() const
{

  return uidCreation_;

}


OFBool DJ2KCodecParameter::getConvertToSC() const
{

  return convertToSC_;

}
