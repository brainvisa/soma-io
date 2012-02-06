
#include <soma/image_stream.hpp>

int main( int, char *[] )
{
//   storage_identifier = soma::StorageIdentifier();
//   storage_identifier.setFormat( "NIFTI-1" );
//   storage_identifier.setFileName( "/tmp/test.nii" );
  // Get a write for the output URI
//   auto writer = soma::image_writer( storage_identifier );
  // Get an image output stream from the writer
  soma::ImageOStream *image_ostream; // = dynamic_cast<soma::ImageOStream *>( writer.ostream() );
  // Write the image header from Aims volume
//   image_ostream->set_binary( );
  return 0;
}