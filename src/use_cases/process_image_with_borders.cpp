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
  // Create an Aims volume with border
  auto image_dimension = image_stream->image_dimension();
  carto::Volume< s16_t > image( image_dimension[0] + 2 * border_size,
                                image_dimension[1] + 2 * border_size, 
                                image_dimension[2] + 2 * border_size );
  // Copy the image header in Aims
  aims::import_metadata( image, image_stream );
  // Create a MemoryLayout describing the organization of the voxels in Aims volume
  soma::MemoryLayout memory_layout;
  memory_layout.set_axis( 0, image_dimension[0], 1 );
  memory_layout.set_axis( 1, image_dimension[1], 2 * border_size );
  memory_layout.set_axis( 2, image_dimension[2], 2 * border_size + 2 * border_size * ( image_dimension[0] + 2 * border_size ) );
  // initialize all image voxels
  image = 0;
  // Ask soma-io to copy the voxels in Aims volume
  image_istream->copy_voxels_in_memory( &image.at( border_size, border_size, border_size ), memory_layout );

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