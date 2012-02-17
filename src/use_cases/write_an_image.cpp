
#include <soma/image_stream.hpp>

using namespace std;
using namespace soma;

int main( int, char *[] )
{
//   storage_identifier = soma::StorageIdentifier();
//   storage_identifier.setFormat( "NIFTI-1" );
//   storage_identifier.setFileName( "/tmp/test.nii" );
  // Get a write for the output URI
//   auto writer = soma::image_writer( storage_identifier );
  // Get an image output stream from the writer
  ImageOStream *image; // = dynamic_cast<soma::ImageOStream *>( writer.ostream() );
  // Write the image header from Aims volume
  BinaryOStream *voxels = image->binary_ostream( ImageFields::voxels );
  voxels->set_type_code( SInt16 );
  voxels->set_size( 256, 256, 512 );
  for( int z = 0; z < 512; ++z ) {
    vector< SInt16_t > data( 256 * 256 );
    voxels->copy_from_memory( &data[0], 256 * 256 );
  }
  vector<double> voxel_size;
  voxel_size.push_back( 1.2 );
  voxel_size.push_back( 1.2 );
  voxel_size.push_back( 0.75 );
  image->set_double_vector( ImageFields::voxel_size, voxel_size );
  return 0;
}