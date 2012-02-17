#include <soma/image_stream.hpp>
#include <cartodata/volume/volume.h>

using namespace std;
using namespace soma;
using namespace carto;

int main( int argc, char *argv[] )
{
  const int border_size = 2;

  #if 0
  // Get a reader from the input URI
  auto reader = soma::image_reader( argv[ 1 ] );
  // Get an image input stream from the reader
  auto image_istream = dynamic_cast<soma::ImageIStream *>( reader.istream() );
  #endif
  ImageIStream *image_in;
  
  BinaryIStream *voxels = image_in->get_binary_stream( ImageFields::voxels );
  vector<size_t> image_size = voxels->size();
  Volume< SInt16_t > *image;
  if ( voxels->allow_memory_mapping() ) {
    // Create a MemoryLayout describing the organization of the voxels in image stream (i.e. on disk)
    MemoryLayout memory_layout = voxels->memory_layout();
    // Create an Aims volume using memory mapping
    image = new carto::Volume< SInt16_t >( image_size[0],
                                           image_size[1], 
                                           image_size[2],
                                           image_size[3],
                                           AllocatorContext( AllocatorStrategy::ReadOnly,
                                                             rc_ptr<DataSource>( new BufferDataSource( (char *) voxels->memory_mapping().first ) ) ) );
  } else {
    image = new carto::Volume< SInt16_t >( image_size[0],
                                           image_size[1], 
                                           image_size[2],
                                           image_size[3] );
    // Create a MemoryLayout describing the organization of the voxels in Aims volume
    soma::MemoryLayout memory_layout( 4 );
    memory_layout.set_axis( 0, image_size[0], 1 );
    memory_layout.set_axis( 1, image_size[1], 0 );
    memory_layout.set_axis( 2, image_size[2], 0 );
    memory_layout.set_axis( 3, image_size[3], 0 );
    // Ask soma-io to copy the voxels in Aims volume
    voxels->copy_in_memory( &image->at( 0 ), memory_layout );
  }
  // Copy the image header in Aims
//   aims::import_metadata( image, image_stream );

  // Do something on image with Aims
//   aims::do_something_on_image_with_border( image );

  // Get a write for the output URI
  //auto writer = soma::image_writer( reader.storage_identifier() );
//   // Get an image output stream from the writer
//   auto image_ostream = dynamic_cast<soma::ImageOStream *>( writer.ostream() );
  // Write the image header from Aims volume
//   aims::export_metadata( image, image_stream );
  // Write the image body
//   image_ostream->copy_voxels_from_memory( &image.at( border_size, border_size, border_size ), memory_layout );
}