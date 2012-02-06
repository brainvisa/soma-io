#include <soma/image_stream.hpp>

namespace soma {

FieldsDefinition ImageFields::_instance;

const FieldsDefinition &ImageFields::instance()
{
  if ( _instance.count() == 0 ) {
    // Initialize standard fields
    _instance.add_field( voxels, "voxels", Binary, "Voxels data" );
    _instance.add_field( voxel_size, "voxel_size", Vector, TypeDescriptor( UInt32 ), "Size of the image in each dimension" );
  }
  return _instance;
}

ImageIStream::~ImageIStream()
{
}

} // namespace soma
