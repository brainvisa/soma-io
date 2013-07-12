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

#ifndef SOMAIO_READER_FORMATREADER_D_H
#define SOMAIO_READER_FORMATREADER_D_H
//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/reader/formatreader.h>                  // class declaration
#include <soma-io/datasource/datasource.h>            // for templating rc_ptr
#include <soma-io/utilities/creator.h>         // used by setup() and create()
#include <soma-io/allocator/allocator.h>                   // AllocatorContext
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
#include <cartobase/smart/rcptr.h>               // reference counting pointer
//--- system -----------------------------------------------------------------
#include <memory>
//----------------------------------------------------------------------------

namespace soma
{
  //==========================================================================
  //   C O N S T R U C T O R S
  //==========================================================================
  template<typename T>
  FormatReader<T>::~FormatReader()
  {
  }
  
  //==========================================================================
  //   N E W   M E T H O D S
  //==========================================================================
  template<typename T>
  void FormatReader<T>::setupAndRead( T & obj, 
                                      carto::rc_ptr<DataSourceInfo> dsi,
                                      const AllocatorContext & context,
                                      carto::Object options )
  {
    setup( obj, dsi->header(), context, options );
    read( obj, dsi, context, options );
  }

  template<typename T>
  T* FormatReader<T>::createAndRead( carto::rc_ptr<DataSourceInfo> dsi,
                                     const AllocatorContext & context,
                                     carto::Object options )
  {
    std::auto_ptr<T>  objp( create( dsi->header(), context, options ) );
    T  *obj = objp.get();
    read( *obj, dsi, context, options );
    objp.release();
    return obj;
  }

  /*** read ******************************************************************
   * This method is totally abstract and should be implemented in a format-
   * specific FormatReader.
   * This implem is only used for FormatReader<GenericObject>
   **************************************************************************/
  template<typename T>
  void FormatReader<T>::read( T & obj, 
                              carto::rc_ptr<DataSourceInfo> dsi, 
                              const AllocatorContext & context, 
                              carto::Object options )
  {
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       context.dataSource() ? 
                                       context.dataSource()->url() : "" );
  }

  //==========================================================================
  //   S T I L L   U S E D
  //==========================================================================

  template<typename T>
  T* FormatReader<T>::create( carto::Object header, 
                              const AllocatorContext & context, 
                              carto::Object options )
  {
    return Creator<T>::create( header, context, options );
  }

  template<typename T>
  void FormatReader<T>::setup( T & obj, carto::Object header, 
                               const AllocatorContext & context, 
                               carto::Object options )
  {
    Creator<T>::setup( obj, header, context, options );
  }

  //==========================================================================
  //   O L D   M E T H O D S
  //==========================================================================

  template<typename T>
  void FormatReader<T>::setupAndRead( T & obj, carto::Object header, 
                                      carto::rc_ptr<DataSource> ds, 
                                      const AllocatorContext & context, 
                                      carto::Object options )
  {
    carto::rc_ptr<DataSource> decoder = getDataSource( header, ds, options );
    AllocatorContext  ac = getAllocatorContext( header, decoder, 
                                                      context, options );
    setup( obj, header, ac, options );
    read( obj, header, ac, options );
  }

  template<typename T>
  T* FormatReader<T>::createAndRead( carto::Object header, 
                                     carto::rc_ptr<DataSource> ds, 
                                     const AllocatorContext & context, 
                                     carto::Object options )
  {
    carto::rc_ptr<DataSource> decoder = getDataSource( header, ds, options );
    AllocatorContext  ac = getAllocatorContext( header, decoder, 
                                                      context, options );
    std::auto_ptr<T>  objp( create( header, ac, options ) );
    T     *obj = objp.get();
    read( *obj, header, ac, options );
    objp.release();
    return obj;
  }

  template<typename T>
  carto::rc_ptr<DataSource> 
  FormatReader<T>::getDataSource( carto::Object, 
                                  carto::rc_ptr<DataSource> source, 
                                  carto::Object )
  {
    return source;
  }

  template<typename T>
  AllocatorContext 
  FormatReader<T>::getAllocatorContext( carto::Object, 
                                        carto::rc_ptr<DataSource> decoder, 
                                        const AllocatorContext & basecontext, 
                                        carto::Object )
  {
    AllocatorContext  alloc( basecontext );
    alloc.setDataSource( decoder );
    alloc.setAllowsMemoryMapping( decoder->allowsMemoryMapping() );
    return alloc;
  }

  /*** read ******************************************************************
   * This method is totally abstract and should be implemented in a format-
   * specific FormatReader.
   * This implem is only used for FormatReader<GenericObject>
   **************************************************************************/
  template<typename T>
  void FormatReader<T>::read( T &, carto::Object, 
                              const AllocatorContext & context, 
                              carto::Object )
  {
    throw carto::invalid_format_error( "format reader not implemented yet...", 
                                       context.dataSource() ? 
                                       context.dataSource()->url() : "" );
  }
  
}

#endif
