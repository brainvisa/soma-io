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
//--- debug ------------------------------------------------------------------
#include <cartobase/config/verbose.h>
#include <cartobase/type/string_conversion.h>
#define localMsg( message ) cartoCondMsg( 4, message, "FORMATREADER" )
// localMsg must be undef at end of file
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
    dsi = checkDataSourceInfo( dsi, options );
    // copy context with compatible mmap mode
    localMsg("Copying allocator context...")
    AllocatorContext ac( context.accessMode(), dsi, context.useFactor() );
    setup( obj, dsi->header(), ac, options );
    read( obj, dsi, ac, options );
  }

  template<typename T>
  T* FormatReader<T>::createAndRead( carto::rc_ptr<DataSourceInfo> dsi,
                                     const AllocatorContext & context,
                                     carto::Object options )
  {
    dsi = checkDataSourceInfo( dsi, options );
    // copy context with compatible mmap mode
    AllocatorContext ac( context.accessMode(), dsi, context.useFactor() );
    std::auto_ptr<T>  objp( create( dsi->header(), ac, options ) );
    T  *obj = objp.get();
    read( *obj, dsi, ac, options );
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


  template<typename T>
  carto::rc_ptr<DataSourceInfo> FormatReader<T>::checkDataSourceInfo(
      carto::rc_ptr<DataSourceInfo> dsi, carto::Object options )
  {
    localMsg("Checking data source informations...")
    // copy options dict to avoid modifying it
    // (it would kill thread safety)
    carto::Object new_options( options->clone() );
    if( new_options.isNone() )
      new_options = carto::Object::value( carto::PropertySet() );
    new_options->setProperty( "format", formatID() );
    DataSourceInfoLoader dsil;
    // check with exact format (pass 0 only)
    DataSourceInfo new_dsi = dsil.check( *dsi, new_options, 0, 0 );
    // FIXME: this copy is suboptimal. dsil.check() should work with rc_ptr
    return carto::rc_ptr<DataSourceInfo>( new DataSourceInfo( new_dsi ) );
  }

  //==========================================================================
  //   S T I L L   U S E D
  //==========================================================================

  template<typename T>
  T* FormatReader<T>::create( carto::Object header, 
                              const AllocatorContext & context, 
                              carto::Object options )
  {
    localMsg("Creating object ...");
    return Creator<T>::create( header, context, options );
  }

  template<typename T>
  void FormatReader<T>::setup( T & obj, carto::Object header, 
                               const AllocatorContext & context, 
                               carto::Object options )
  {
    localMsg("Setting object ...");
    Creator<T>::setup( obj, header, context, options );
  }

}

#undef localMsg
#endif
