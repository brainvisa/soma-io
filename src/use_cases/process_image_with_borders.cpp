#error This is a pseudo-code example

#include <all-i-need-from-soma>
#include <all-i-need-from-aims>

int main( int argc, char *argv[] )
{
  const int border_size = 2;

  auto reader = soma::image_reader( argv[ 1 ] );
  auto image_istream = dynamic_cast<soma::ImageIStream *>( reader.istream() );
  auto image_dimension = image_stream->image_dimension();
  carto::Volume< s16_t > image( image_dimension[0] + 2 * border_size,
                                image_dimension[1] + 2 * border_size, 
                                image_dimension[2] + 2 * border_size );
  aims::import_metadata( image, image_stream );
  soma::MemoryLayout memory_layout;
  memory_layout.set_axis( 0, image_dimension[0], 1 );
  memory_layout.set_axis( 1, image_dimension[0], 2 * border_size );
  memory_layout.set_axis( 2, image_dimension[0], 2 * border_size + 2 * border_size * ( image_dimension[0] + 2 * border_size ) );
  image_istream->copy_voxels_in_memory( &image.at( border_size, border_size, border_size ), memory_layout );

  aims::do_something_on_image_with_border( image );

  auto writer = soma::image_writer( reader.storage_identifier() );
  auto image_ostream = dynamic_cast<soma::ImageOStream *>( writer.ostream() );
  aims::export_metadata( image, image_stream );
  image_ostream->copy_voxels_from_memory( &image.at( border_size, border_size, border_size ), memory_layout );
}