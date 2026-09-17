
#include <cartobase/stream/attachable_cuifstream.h>

using namespace carto;
using namespace std;


attachable_cuifstream::attachable_cuifstream()
  : cuifstream(), _attached( 0 )
{
}


attachable_cuifstream::attachable_cuifstream( const char *name ) 
  : cuifstream( name ), _attached( 0 )
{
}


attachable_cuifstream::attachable_cuifstream( istream & s )
  : cuifstream(), _attached( &s )
{
  init_buffers();
}


attachable_cuifstream::~attachable_cuifstream()
{
  close();
}


void attachable_cuifstream::attach( istream & s, int line_num )
{
  //if( !_attached )
  close();
    //else
    //destroy_buffers();
  _attached = &s;
  init_buffers();
  setstate( _attached->rdstate() );
  _counter->extractor().reset( line_num );
}


void attachable_cuifstream::close()
{
  cuifstream::close();
  _attached = 0;
}


void attachable_cuifstream::init_buffers()
{
  if( _attached )
    {
      rdbuf( _attached->rdbuf() );
      _counter = new fistreambuf<CounterExtractor>( rdbuf() );
      rdbuf( _counter );
      _uncomment = new fistreambuf<UncommentExtractor>( rdbuf() );
      rdbuf( _uncomment );
    }
  else
    cuifstream::init_buffers();
}


void attachable_cuifstream::destroy_buffers()
{
  if( _attached )
    {
      _attached->setstate( rdstate() );
      _attached = 0;
    }
  cuifstream::destroy_buffers();
}


bool attachable_cuifstream::operator!() const
{
  if( _attached )
    return _attached->operator!() || istream::operator!();
  else
    return( cuifstream::operator!() );
}


bool attachable_cuifstream::is_open() const
{
  if( _attached )
    {
      ifstream	*fs = dynamic_cast<ifstream *>( _attached );
      if( fs )
	return fs->is_open();
      return( true );	// non-file streams are considered always open
    }
  else
    return( cuifstream::is_open() );
}


