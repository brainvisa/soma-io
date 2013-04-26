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

#ifndef CARTOBASE_IO_READER_H
#define CARTOBASE_IO_READER_H
//--- members -----------------------------------------------------------------
#include <somaio/datasource/datasourcelist.h>
#include <cartobase/allocator/allocator.h>
#include <cartobase/object/object.h>
//--- system -------------------------------------------------------------------
#include <string>
//------------------------------------------------------------------------------

namespace carto
{
  
  /**
   * Generic reader for *every* format of Cartograph object.
   * Still a template class, the data type must be switched at upper-level 
   * (see DataSourceInfoLoader or ReaderAlgorithm).\n
   * Generic readers offer a plug-in mechanism to allow new formats to 
   * be registered.\n
   * It uses at lower level specific readers of each object type (Volume,
   * Meshes, Buckets, Textures) and even lower, specific reader of each
   * format (GIS, SPM)
   *
   * \par Adding new readers
   *
   * The standard cartodata library includes a set of already compiled 
   * formats, using a FormatDictionary to provide a common link between Reader 
   * and Writer, see FormatDictionary for a list.
   *
   * If you need to use other types, you'll have to include 
   * \c cartobase/io/reader_d.h in order to have the template functions 
   * definitions, \c cartobase/io/reader.h is only the class declarations.
   * In addition, you may want to specialize the registerBaseFormats() 
   * functions of the FormatDictionary classes.
   *
   * \see FormatDictionary Writer DataSourceInfo ReaderAlgorithm
   */
  template <typename T>
  class Reader
  {
  public:
    //--- constructors ---------------------------------------------------------
    Reader();
    Reader( const std::string & filename );
    Reader( rc_ptr<DataSource> ds );
    Reader( std::istream & stream );
    virtual ~Reader();
    
    //--- read methods ---------------------------------------------------------
    /**
     * \brief Finds the correct format and reads the object. 
     * if \c format is specified, this format is tried first, so you can use it
     * as a hint if you already know it (from the DataSourceInfoLoader check ).
     */
    virtual bool read( T & obj, Object header = none() );
    /**
     * \brief Creates and reads an object. 
     * This function differs from the read( T&, ... ) function in the way 
     * that it creates the object and does not just fill it. This enables 
     * to create sub-classes of T (factory-like behaviour) on types that 
     * allow subclasses. The object is created by \c new and can be deleted. 
     * The default is just to create a T and call the read( T&, ... ) function, 
     * but some specialized low-level readers may behave differently.
     */
    virtual T* read( Object header = none() );
    
    //--- allocator ------------------------------------------------------------
    void setAllocatorContext( const AllocatorContext & ac );
    const AllocatorContext & allocatorContext() const;
    
    //--- options --------------------------------------------------------------
    void setOptions( Object options );
    Object   options() const;
    Object & options();
    
    //--- datasource -> manipulates "default" datasource -----------------------
    const rc_ptr<DataSource> dataSource() const;
          rc_ptr<DataSource> dataSource();
    void attach( rc_ptr<DataSource> ds );
    void attach( const std::string & filename, offset_t offset = 0 );
    void attach( std::istream & stream );
    
    //--- data source control --------------------------------------------------
    // maybe add flushAll() and closeAll() ?
    void flush();
    void close();
    
    //--- useful ---------------------------------------------------------------
    // is it actually used or implemented somewhere ?
    static std::string extension( const std::string & filename );
    
    //--- new : datasourcelist -------------------------------------------------
    const DataSourceList & dataSourceList() const { return _datasourcelist; }
          DataSourceList & dataSourceList()       { return _datasourcelist; }

  protected:
    DataSourceList    _datasourcelist;
    AllocatorContext  _alloccontext;
    Object            _options;
  };
}

//==============================================================================
//   S T R E A M   O P E R A T O R
//==============================================================================
template <class T>
inline carto::Reader<T> &
operator >> ( carto::Reader<T> & reader, T & thing )
{
  reader.read( thing );
  return reader;
}

#endif

