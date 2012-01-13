#error This is a pseudo-code example

#include <all-i-need-from-soma>
#include <all-i-need-from-aims>

int main( int argc, char *argv[] )
{
  const int border_size = 2;

  // Get a reader from the input URI
  auto reader = soma::image_reader( argv[ 1 ] );
  // Get an image input stream from the reader
  auto image_istream = dynamic_cast<soma::ImageIStream *>( reader.istream() );
  
  auto image_dimension = image_stream->image_dimension();
  carto::Volume< s16_t > *image;
  if ( image_stream->allow_memory_mapping() ) {
    // Create a MemoryLayout describing the organization of the voxels in image stream (i.e. on disk)
    soma::MemoryLayout memory_layout = image_stream->memory_layout();
    // Create an Aims volume using memory mapping
    image = new carto::Volume< s16_t >( image_dimension[0],
                                        image_dimension[1], 
                                        image_dimension[2],
                                        image_dimension[3],
                                        AllocatorContect( AllocatorStrategy::ReadOnly,
                                                          new BufferDataSource( image_stream->memory_mapping() ) );
  } else {
    image = new carto::Volume< s16_t >( image_dimension[0],
                                        image_dimension[1], 
                                        image_dimension[2],
                                        image_dimension[3] );
    // Create a MemoryLayout describing the organization of the voxels in Aims volume
    soma::MemoryLayout memory_layout;
    memory_layout.set_axis( 0, image_dimension[0], 1 );
    memory_layout.set_axis( 1, image_dimension[1], 0 );
    memory_layout.set_axis( 2, image_dimension[2], 0 );
    memory_layout.set_axis( 3, image_dimension[3], 0 );
    // Ask soma-io to copy the voxels in Aims volume
    image_istream->copy_voxels_in_memory( &image.at( 0 ), memory_layout );
  }
  // Copy the image header in Aims
  aims::import_metadata( image, image_stream );

  // Do something on image with Aims
  aims::do_something_on_image_with_border( image );

  // Get a write for the output URI
  auto writer = soma::image_writer( reader.storage_identifier() );
  // Get an image output stream from the writer
  auto image_ostream = dynamic_cast<soma::ImageOStream *>( writer.ostream() );
  // Write the image header from Aims volume
  aims::export_metadata( image, image_stream );
  // Write the image body
  image_ostream->copy_voxels_from_memory( &image.at( border_size, border_size, border_size ), memory_layout );
}