//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/stream/cuifstream.h>

using namespace carto;
using namespace std;


//=============================================================================
//	M E T H O D S
//=============================================================================

cuifstream::cuifstream()
  : istream(0), _counter(0), _uncomment(0)
{
}


cuifstream::cuifstream(const char* name, cuifstream::openmode mode)
  : istream(0), _base( name, mode ), _enableuncomment( true )
{
  init_buffers();
}


cuifstream::~cuifstream()
{
  destroy_buffers();
}


void
cuifstream::init_buffers()
{
  iostate s = _base.rdstate();
  rdbuf(_base.rdbuf());
  _counter = new fistreambuf<CounterExtractor>(rdbuf());
  rdbuf(_counter);
  if( _enableuncomment )
  {
    _uncomment = new fistreambuf<UncommentExtractor>(rdbuf());
    rdbuf(_uncomment);
  }
  _base.setstate( s );
  setstate( s );
}


void
cuifstream::destroy_buffers()
{
  delete _uncomment;
  _uncomment = 0;
  delete _counter;
  _counter = 0;
}


void
cuifstream::open(const char* filename, cuifstream::openmode mode)
{
  destroy_buffers();
  istream::clear();
  _base.clear();
  _base.open(filename, mode);
  init_buffers();
}


void
cuifstream::close()
{
  _base.close();
  destroy_buffers();
}


bool
cuifstream::operator!() const
{
  return _base.operator!() || istream::operator!();
}


bool
cuifstream::is_open() const
{
  return _base.is_open();
}


void cuifstream::enableUncomment( bool x )
{
  _enableuncomment = x;
  istream::clear();
  _base.clear();
  destroy_buffers();
  init_buffers();
}


bool
cuifstream::eof() const
{
#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
  // in gcc 4.6, the eof bit does never get set; fail does however.
  return _base.eof() || istream::eof() || istream::fail();
#else
  return _base.eof() || istream::eof();
#endif
}


