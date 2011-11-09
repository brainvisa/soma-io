#ifndef SOMA_IMAGE_STREAM
#define SOMA_IMAGE_STREAM

#include <soma/object_stream.hpp>

namespace soma {

class ImageStream : public ObjectStream
{
public:
  virtual ~ImageStream();
  
  virtual TypeCode voxel_type_code() const = 0;
  virtual const TypeDescriptor *voxel_type_descriptor() const = 0;
  
  const vector<size_t> &image_dimension() const = 0;
  const vector<double> &voxel_size() const = 0;
  
  // Voxels
  // Memory mapping par soma-io (si possible) -> allocation par soma-io et désallocation par le client
  // Pointage de mémoire déja allouée par soma-io -> pas d'allocation / désallocation
  // Remplissage de mémoire allouée par le client
  
  
};


} // namespace soma

#endif // SOMA_IMAGE_STREAM
