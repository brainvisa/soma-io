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


// Reader for GIS image format

#ifndef SOMA_GIS_FORMAT_READER
#define SOMA_GIS_FORMAT_READER

#include <string>

#include <soma/wip/image_reader.hpp>


namespace soma {


class GisFormatReader
{
public:
  GisFormatReader( const std::string &file_name );

  // Image size
  inline int image_size_x()
  {
    read_header();
    return sx;
  }
  inline int image_size_y()
  {
    read_header();
    return sy;
  }
  inline int image_size_z()
  {
    read_header();
    return sz;
  }
  inline int image_size_t()
  {
    read_header();
    return st;
  }

  // Image data
  inline ImageReader::DataType voxel_type()
  {
    read_header();
    return vtype;
  }
  
  void voxels( void *, int offset_x, int offset_y, int offset_z, int offset_t );
  
  // Voxel size
  // Image size
  inline int voxel_size_x()
  {
    read_header();
    return vx;
  }
  inline int voxel_size_y()
  {
    read_header();
    return vy;
  }
  inline int voxel_size_z()
  {
    read_header();
    return vz;
  }
  inline int voxel_size_t()
  {
    read_header();
    return vt;
  }

private:

  void read_header();
  
  bool header_not_read;
  int sx, sy, sz, st;
  ImageReader::DataType vtype;
  double vx, vy, vz, vt;
};


extern template class ImageReaderOf< GisFormatReader >;

} // namespace soma

#endif // ifndef SOMA_GIS_FORMAT_READER
