#ifndef SOMA_IMAGE_STREAM
#define SOMA_IMAGE_STREAM

#include <vector>
#include <soma/object_stream.hpp>
#include <soma/memory_layout.hpp>


namespace soma {


class ImageFields : FieldsDefinition
{
public:
  static const FieldIndex voxels = 1;
  static const FieldIndex voxel_size = 2;
  static const FieldIndex non_standard_header = 3;
  
  static const FieldsDefinition &instance();

private:
  static FieldsDefinition _instance;
};


class ImageIStream : public FieldIStream
{
public:
  static const FieldIndex voxels = 1;
  static const FieldIndex voxel_size = 2;
  static const FieldIndex non_standard_header = 3;

  virtual ~ImageIStream();
    
  // Voxels
  // Memory mapping par soma-io (si possible) -> allocation par soma-io et désallocation par le client
  // Pointage de mémoire déja allouée par soma-io -> pas d'allocation / désallocation
  // Remplissage de mémoire allouée par le client

protected:
  static FieldsDefinition _image_standard_fields;
  
};



class ImageOStream : public FieldOStream
{
public:

  inline void set_voxel_size( double size1 )
  {
    std::vector< double > voxel_size;
    voxel_size.push_back( size1 );
    set_double_vector( ImageFields::voxel_size, voxel_size );
  }
  
  
  inline void set_voxel_size( double size1, double size2 )
  {
    std::vector< double > voxel_size;
    voxel_size.push_back( size1 );
    voxel_size.push_back( size2 );
    set_double_vector( ImageFields::voxel_size, voxel_size );
  }
  
  
  inline void set_voxel_size( double size1, double size2, double size3 )
  {
    std::vector< double > voxel_size;
    voxel_size.push_back( size1 );
    voxel_size.push_back( size2 );
    voxel_size.push_back( size3 );
    set_double_vector( ImageFields::voxel_size, voxel_size );
  }
};


} // namespace soma

#endif // SOMA_IMAGE_STREAM
