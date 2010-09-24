/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

// Memory independent image reader. To be discussed.

#ifndef SOMA_IMAGE_READER
#define SOMA_IMAGE_READER

namespace soma {


class ImageReader
{
public:

  typedef enum
  {
    U8, S8, U16, S16, U32, S32, RGB, RGBA
  } DataType;
  
  // Image size
  virtual int image_size_x() = 0;
  virtual int image_size_y() = 0;
  virtual int image_size_z() = 0;
  virtual int image_size_t() = 0;
  
  // Image data
  virtual DataType voxel_type() = 0;
  virtual void voxels( void *, int offset_x, int offset_y, int offset_z, int offset_t ) = 0;
  
  // Voxel size
  virtual double voxel_size_x() = 0;
  virtual double voxel_size_y() = 0;
  virtual double voxel_size_z() = 0;
  virtual double voxel_size_t() = 0;
};



template < class FormatReader >
class ImageReaderOf : ImageReader
{
public:
  inline ImageReaderOf( FormatReader &format_reader ) : format( format_reader ) {}
  
  inline int image_size_x() { return format.image_size_x(); }
  inline int image_size_y() { return format.image_size_y(); }
  inline int image_size_z() { return format.image_size_z(); }
  inline int image_size_t() { return format.image_size_t(); }
  
  // Image data
  inline DataType voxel_type() { return format.voxel_type(); }
  inline void voxels( void *pointer, int offset_x, int offset_y, int offset_z, int offset_t )
  {
    return format.voxels( pointer, offset_x, offset_y, offset_z, offset_t );
  }
  
  // Voxel size
  inline double voxel_size_x() { return format.voxel_size_x(); }
  inline double voxel_size_y() { return format.voxel_size_y(); }
  inline double voxel_size_z() { return format.voxel_size_z(); }
  inline double voxel_size_t() { return format.voxel_size_t(); }

private:

  FormatReader &format;
};


} // namespace soma

#endif // ifndef SOMA_IMAGE_READER
