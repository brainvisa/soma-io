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

// Memory independent image writer. To be discussed.

#ifndef SOMA_IMAGE_WRITER
#define SOMA_IMAGE_WRITER

#include <soma/wip/image_reader.hpp>

namespace soma {


class ImageWriter
{
public:
  // Image size
  virtual void image_size_x( int ) = 0;
  virtual void image_size_y( int ) = 0;
  virtual void image_size_z( int ) = 0;
  virtual void image_size_t( int ) = 0;
  
  // Image data
  virtual void voxel_type( ImageReader::DataType ) = 0;
  virtual void voxels( void *, int offset_x, int offset_y, int offset_z, int offset_t ) = 0;
  
  // Voxel size
  virtual void voxel_size_x( double ) = 0;
  virtual void voxel_size_y( double ) = 0;
  virtual void voxel_size_z( double ) = 0;
  virtual void voxel_size_t( double ) = 0;
};



template < class FormatWriter >
class ImageWriterOf : ImageWriter
{
public:
  inline ImageWriterOf( FormatWriter &format_writer ) : format( format_writer ) {}
  
  inline void image_size_x( int size ) { format.image_size_x( size ); }
  inline void image_size_y( int size ) { format.image_size_y( size ); }
  inline void image_size_z( int size ) { format.image_size_z( size ); }
  inline void image_size_t( int size ) { format.image_size_t( size ); }
  
  // Image data
  inline voxel_type( ImageReader::DataType type ) { format.voxel_type( type ); }
  inline void voxels( void *pointer, int offset_x, int offset_y, int offset_z, int offset_t )
  {
    format.voxels( pointer, offset_x, offset_y, offset_z, offset_t );
  }
  
  // Voxel size
  inline void voxel_size_x( double size ) { format.voxel_size_x( size ); }
  inline void voxel_size_y( double size ) { format.voxel_size_y( size ); }
  inline void voxel_size_z( double size ) { format.voxel_size_z( size ); }
  inline void voxel_size_t( double size ) { format.voxel_size_t( size ); }

private:

  FormatWriter &format;
};


} // namespace soma

#endif // ifndef SOMA_IMAGE_WRITER
