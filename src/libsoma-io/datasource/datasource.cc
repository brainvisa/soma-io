//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                 // class definition
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                     // none()'s return type
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

const rc_ptr<DataSource> DataSource::none()
{
  static const rc_ptr<DataSource>	n;
  return n;
}


DataSource::DataSource()
  : RCObject(), _mode( Read )
{
}


DataSource::~DataSource()
{
}


int DataSource::mode() const
{
  return _mode;
}


bool DataSource::atEnd() const
{
  return at() == size() - 1;
}


bool DataSource::reset()
{
  close();
  open( _mode );
  return isOpen();
}


void DataSource::flush()
{
}


std::string DataSource::url() const
{
  return string();
}


bool DataSource::eof() const
{
  return at() >= size();
}


bool DataSource::operator ! () const
{
  return !isOpen() || eof();
}


DataSource::operator bool () const
{
  return ! operator ! ();
}


