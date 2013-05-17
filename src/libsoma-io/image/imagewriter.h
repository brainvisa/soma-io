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

#ifndef SOMAIO_IMAGE_IMAGEWRITER_H
#define SOMAIO_IMAGE_IMAGEWRITER_H
//--- soma-io ------------------------------------------------------------------
#include <soma-io/datasourceinfo/datasourceinfo.h>                     // member
//--- cartobase ----------------------------------------------------------------
#include <cartobase/object/object.h>                            // to use none()
//--- system -------------------------------------------------------------------
#include <vector>
//------------------------------------------------------------------------------

namespace soma
{
  
  /// ImageWriter is a low level Image/Volume writer.
  ///
  /// Format-specific writers ( GIS, OpenSlide, ... ) are derived from it.
  /// They allow ( if implemented ) partial writing (and multiresolution ? ).
  template<typename T>
  class ImageWriter
  {
    public:
      ImageWriter();
      virtual ~ImageWriter();
      
      /// Reading a region of a Image/Volume at a given resolution to a 
      /// pre-allocated buffer. Positions are expressed in 4D (x,y,z,t). If one 
      /// or more of these dimensions are of no interest for the format, they
      /// take the value 0 ( pos ) or 1 ( size )
      /// \param source Buffer containing the region to write. It must be of
      ///               length size[0]*size[1]*size[2]*size[3]*sizeof(T)
      /// \param pos    Position of the first voxel of the region to write.
      /// \param size   Size of the region to write.
      /// \param options  ( is it useful here ? )
      virtual void write( T * source, DataSourceInfo & dsi,
                          std::vector<int> & pos,  /* taille 4 : x,y,z,t */
                          std::vector<int> & size, /* taille 4 : x,y,z,t */
                          int level = 0, 
                          carto::Object options = carto::none() );
      
      /// Builds the DataSourceList
      /// Checks whether the files need to be created and eventually does so.
      /// (It is needed for partial writing)
      virtual DataSourceInfo writeHeader( DataSourceInfo dsi, 
                                          carto::Object options = carto::none() );
      
      /// Sets _sizes, _binary and _byteswap values from DataSourceInfo
      virtual void updateParams( DataSourceInfo & dsi );
      /// Sets _sizes, _binary and _byteswap values as default or empty
      virtual void resetParams();
      
    protected:
      std::vector<std::vector<int> >  _sizes;  //  4D : x, y, z, t
      bool  _binary;        // default: true
      bool  _byteswap;      // default: false
  };
  
}

#endif