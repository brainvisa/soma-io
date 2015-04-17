#ifdef SOMA_IO_DICOM
#include <soma-io/Dicom/PaletteModule.h>
#else
#include <Dicom/PaletteModule.h>
#endif

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcitem.h>
#include <dcmtk/dcmdata/dcdeftag.h>


dcm::PaletteModule::PaletteModule()
                  : dcm::DicomModule()
{
}


bool dcm::PaletteModule::parseItem( DcmItem* dcmItem )
{

  if ( dcmItem )
  {

    _red.initialize( dcmItem, 
                     DCM_RedPaletteColorLookupTableDescriptor,
                     DCM_RedPaletteColorLookupTableData );
    _green.initialize( dcmItem,
                       DCM_GreenPaletteColorLookupTableDescriptor,
                       DCM_GreenPaletteColorLookupTableData );
    _blue.initialize( dcmItem,
                      DCM_BluePaletteColorLookupTableDescriptor,
                      DCM_BluePaletteColorLookupTableData );

    return true;

  }

  return false;

}


dcm::Rgba dcm::PaletteModule::operator()( int32_t index )
{

  uint8_t r, g, b;

  if ( _red.getBits() == 8 )
  {

    r = uint8_t( _red( index ) & 0xff );
    g = uint8_t( _green( index ) & 0xff );
    b = uint8_t( _blue( index ) & 0xff );

  }
  else
  {

    r = uint8_t( ( _red( index ) & 0xff00 ) >> 8 );
    g = uint8_t( ( _green( index ) & 0xff00 ) >> 8 );
    b = uint8_t( ( _blue( index ) & 0xff00 ) >> 8 );

  }

  return dcm::Rgba( r, g, b );

}
