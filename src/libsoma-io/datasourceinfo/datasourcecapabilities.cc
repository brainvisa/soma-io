//--- soma io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourcecapabilities.h>      // declaration
#include <soma-io/datasource/datasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

//============================================================================
//   C O N S T R U C T O R S
//============================================================================

DataSourceCapabilities::DataSourceCapabilities() : _capabilities( 0 )
{
}

DataSourceCapabilities::DataSourceCapabilities( const DataSourceCapabilities 
                                                & other ) :
  _capabilities( other._capabilities ), _mds( other._mds )
{
}

DataSourceCapabilities::~DataSourceCapabilities()
{
}

void DataSourceCapabilities::reset()
{
  _capabilities = 0;
}

//============================================================================
//   A C C E S S O R S
//============================================================================

bool DataSourceCapabilities::isInit() const
{
  return _capabilities & 1;
}

bool DataSourceCapabilities::allowsMemoryMapping() const
{
  return _capabilities & 2;
}

bool DataSourceCapabilities::isThreadSafe() const
{
  return _capabilities & 4;
}

bool DataSourceCapabilities::isOrdered() const
{
  return _capabilities & 8;
}

bool DataSourceCapabilities::canSeekVoxel() const
{
  return _capabilities & 16;
}

bool DataSourceCapabilities::canSeekLine() const
{
  return _capabilities & 32;
}

bool DataSourceCapabilities::canSeekSlice() const
{
  return _capabilities & 64;
}

bool DataSourceCapabilities::canSeekVolume() const
{
  return _capabilities & 128;
}

bool DataSourceCapabilities::isRandomAccessEfficient() const
{
  return _capabilities & 256;
}

bool DataSourceCapabilities::canHandleStrides() const
{
  return _capabilities & 512;
}

rc_ptr<DataSource>  DataSourceCapabilities::mappableDataSource() const
{
  return _mds;
}

//============================================================================
//   M U T A T O R S
//============================================================================

void DataSourceCapabilities::setInit( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 1;
  else
    _capabilities = _capabilities & (0xffffffff - 1);
}

void DataSourceCapabilities::setMemoryMapping( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 2;
  else
    _capabilities = _capabilities & (0xffffffff - 2);
  setInit();
}

void DataSourceCapabilities::setThreadSafe( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 4;
  else
    _capabilities = _capabilities & (0xffffffff - 4);
  setInit();
}

void DataSourceCapabilities::setOrdered( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 8;
  else
    _capabilities = _capabilities & (0xffffffff - 8);
  setInit();
}

void DataSourceCapabilities::setSeekVoxel( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 16;
  else
    _capabilities = _capabilities & (0xffffffff - 16);
  setInit();
}

void DataSourceCapabilities::setSeekLine( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 32;
  else
    _capabilities = _capabilities & (0xffffffff - 32);
  setInit();
}

void DataSourceCapabilities::setSeekSlice( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 64;
  else
    _capabilities = _capabilities & (0xffffffff - 64);
  setInit();
}

void DataSourceCapabilities::setSeekVolume( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 128;
  else
    _capabilities = _capabilities & (0xffffffff - 128);
  setInit();
}

void DataSourceCapabilities::setRandomAccessEfficient( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 256;
  else
    _capabilities = _capabilities & (0xffffffff - 256);
  setInit();
}

void DataSourceCapabilities::setHandleStrides( const bool & boo )
{
  if( boo )
    _capabilities = _capabilities | 512;
  else
    _capabilities = _capabilities & (0xffffffff - 512);
  setInit();
}

void DataSourceCapabilities::setDataSource( const rc_ptr<DataSource> ds )
{
  _mds = ds;
}
