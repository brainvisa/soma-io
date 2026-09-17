//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/chaindatasource.h>            // class definition
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                            // argument type
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

ChainDataSource::ChainDataSource( rc_ptr<DataSource> ds, const string & url )
  : DataSource(), _source( ds ), _url( url )
{
}


ChainDataSource::~ChainDataSource()
{
}


void ChainDataSource::setSource( rc_ptr<DataSource> ds, const string & url )
{
  close();
  _source = ds;
  _url = url;
}


rc_ptr<DataSource> ChainDataSource::source()
{
  return _source;
}


const rc_ptr<DataSource> ChainDataSource::source() const
{
  return _source;
}


bool ChainDataSource::isOpen() const
{
  return _source && _source->isOpen();
}


bool ChainDataSource::open( int mode )
{
  if( _source )
    return _source->open( mode );
  return false;
}


void ChainDataSource::close()
{
  if( _source )
    _source->close();
}


string ChainDataSource::url() const
{
  return _url;
}


