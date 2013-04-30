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

#ifndef SOMAIO_DATASOURCE_DATASOURCELIST_H
#define SOMAIO_DATASOURCE_DATASOURCELIST_H
//--- somaio -------------------------------------------------------------------
#include <soma-io/datasource/datasource.h>               // to declare rc_ptr of
//--- cartobase ----------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                       // to declare vector of
//--- system -------------------------------------------------------------------
#include <string>
#include <set>
#include <map>
#include <vector>
//------------------------------------------------------------------------------

namespace soma
{
  /// \brief This class allows to manipulate "lists" of pointers to DataSource.
  ///
  /// It has the design of a dictionary in order to sort sources by
  /// content (header, minf, data, ...). Since those contents depend on
  /// the format, the keywords used are defined by specific checkers 
  /// and readers.\n
  /// \see FormatChecker FormatReader
  /// 
  /// The only *global* keyword is "default" which is used to store the
  /// DataSource defining (at construction) a reader, since we don't know at 
  /// this time its content.\n
  /// \see Reader
  /// 
  /// Access to the source is done using dataSource(...) methods.\n
  /// Sources are ordered by increasing order of insertion and numbering 
  /// starts at 0.
  ///
  /// \note For now, if this object is used in the context of reading/writing,
  /// be careful to always have a source pointed by "default" (the source
  /// used at construction). Thus, we know that if the number of keywords is
  /// equal to 1, the *real* list is yet to be built.
  class DataSourceList
  {
    public:
      //========================================================================
      //   C O N S T R U C T O R S
      //========================================================================
      /// Default constructor : builds an empty map
      DataSourceList();
      /// Constructor : builds a 1-element map
      /// This allows to construct easily a 1 element list, useful when
      /// declaring a Reader which creator takes a source as parameter.
      /// \param ds Element to insert
      /// \param type Category of the source
      /// default key is "default" -> to be used for Reader construction
      DataSourceList( const carto::rc_ptr<DataSource> & ds, 
                      const std::string & type = "default" );
      /// Copy constructor
      DataSourceList( const DataSourceList & );
      DataSourceList & operator = ( const DataSourceList & );
      virtual ~DataSourceList();
      
      //========================================================================
      //   A C C E S S O R S / M U T A T O R S
      //========================================================================
      bool operator == ( const DataSourceList & ) const;
      bool operator != ( const DataSourceList & ) const;
      /// Returns true only if no keyword inserted. 
      /// \warning May return false while no DataSource present 
      /// (in theory but in practice ? since we do not allow deletion of a DS)
      bool  empty() const;
      /// Returns existing keywords.
      /// \warning There may be existing keywords with no DataSource
      /// (in theory but in practice ? since we do not allow deletion of a DS)
      std::set<std::string> types() const;
      int   nbTypes() const;
      bool  exists( const std::string & ) const;
      bool  empty( const std::string & ) const;
      int   size( const std::string & ) const;

      /// Accessing an element of the list
      /// If keyword doesn't exist or is empty or coordinate is undefined, 
      /// returns DataSource::none()
      /// Numbering starts at 0
      const carto::rc_ptr<DataSource> & dataSource( const std::string &, int ) const ;
            carto::rc_ptr<DataSource> & dataSource( const std::string &, int );
      /// Adds an element to the dictionary
      /// If new keyword, creates it.
      void addDataSource( const std::string &, const carto::rc_ptr<DataSource> &);
      /// sets the list and keywords empty.
      void reset();
      
    protected:
      std::map<std::string,std::vector<carto::rc_ptr<DataSource> > >  _dslist;
  };
 
}

#endif