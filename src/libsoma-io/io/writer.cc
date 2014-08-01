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

//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/io/writer_d.h>
#include <soma-io/datasource/filedatasource.h>
#include <soma-io/datasource/streamdatasource.h>
#include <soma-io/datasourceinfo/datasourceinfo.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

GenericWriter::GenericWriter()
{
}


GenericWriter::GenericWriter( rc_ptr<DataSource> ds )
  : _datasourceinfo( new DataSourceInfo( ds ) )
{
}


GenericWriter::GenericWriter( const string & filename )
  : _datasourceinfo ( new DataSourceInfo
                      ( rc_ptr<DataSource>
                        ( new FileDataSource( filename ) ) 
                      )
                    )
{
}


GenericWriter::GenericWriter( ostream & stream )
  : _datasourceinfo ( new DataSourceInfo
                      ( rc_ptr<DataSource>
                        ( new OStreamDataSource( stream ) ) 
                      )
                    )
{
}


GenericWriter::~GenericWriter()
{
}


void GenericWriter::flush()
{
  if( _datasourceinfo->list().dataSource() )
    _datasourceinfo->list().dataSource()->flush();
}


void GenericWriter::close()
{
  if( _datasourceinfo->list().dataSource() )
    _datasourceinfo->list().dataSource( )->close();
}


const rc_ptr<DataSource> GenericWriter::dataSource() const
{
  return _datasourceinfo->list().dataSource();
}


rc_ptr<DataSource> GenericWriter::dataSource()
{
  return _datasourceinfo->list().dataSource();
}


void GenericWriter::attach( rc_ptr<DataSource> ds )
{
  _datasourceinfo.reset( new DataSourceInfo( ds ) );
}


void GenericWriter::attach( const std::string & filename )
{
  _datasourceinfo.reset ( new DataSourceInfo
                          ( rc_ptr<DataSource>( new FileDataSource( filename ) ) )
                        );
}


void GenericWriter::attach( ostream & stream )
{
  _datasourceinfo.reset ( new DataSourceInfo
                          ( rc_ptr<DataSource>( new OStreamDataSource( stream ) ) )
                        );
}


template class Writer<GenericObject>;
template class Writer<Object>;

