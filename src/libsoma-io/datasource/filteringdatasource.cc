//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/filteringdatasource.h>        // class definition
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                        // function argument
//--- system -----------------------------------------------------------------
#include <stdio.h>
#include <string>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;

FilteringDataSource::FilteringDataSource( rc_ptr<DataSource> in, int line_num )
  : ChainDataSource( in, in ? in->url() : string() ), 
    _filtercomments( true ), _countlines( true ), _line( line_num ), 
    _commenttag( "#" )
{
}


FilteringDataSource::~FilteringDataSource()
{
}


DataSource* FilteringDataSource::clone() const
{
  FilteringDataSource	*fds
    = new FilteringDataSource( source(), lineNumber() );
  fds->setFiltersComments( filtersComments() );
  fds->setCountsLines( countsLines() );
  return fds;
}


int FilteringDataSource::iterateMode() const
{
  return _source->iterateMode();
}


soma::offset_t FilteringDataSource::size() const
{
  return _source ? _source->size() : 0;
}


soma::offset_t FilteringDataSource::at() const
{
  return _source ? _source->at() : 0;
}


bool FilteringDataSource::at( offset_t pos )
{
  if( _source )
    return _source->at( pos );
  return false;
}


long FilteringDataSource::readBlock( char* data, unsigned long maxlen )
{
  unsigned long i;
  for( i=0; i<maxlen && *_source; ++i )
    *data++ = static_cast<char>( getch() );
  return i;
}


long FilteringDataSource::writeBlock( const char* data, unsigned long len )
{
  return _source->writeBlock( data, len );
}


int FilteringDataSource::getch()
{
  int	c = _source->getch();
  if( c == EOF )
    return c;
  if( _filtercomments && c == _commenttag[0] )
    while( c != '\n' && c != EOF )
      c = _source->getch();
  if( _countlines && c == '\n' )
    ++_line;
  return c;
}


int FilteringDataSource::putch( int ch )
{
  return _source->putch( ch );
}


bool FilteringDataSource::ungetch( int ch )
{
  if( _countlines && ch == '\n' )
    --_line;
  // TODO: handle comment buffer
  return _source->ungetch( ch );
}


bool FilteringDataSource::allowsMemoryMapping() const
{
  return false;
}


bool FilteringDataSource::filtersComments() const
{
  return _filtercomments;
}


bool FilteringDataSource::countsLines() const
{
  return _countlines;
}


void FilteringDataSource::setFiltersComments( bool x )
{
  _filtercomments = x;
}


void FilteringDataSource::setCountsLines( bool x )
{
  _countlines = x;
}


int FilteringDataSource::lineNumber() const
{
  return _line;
}


void FilteringDataSource::setLineNumber( int x )
{
  _line = x;
}


string FilteringDataSource::commentTag() const
{
  return _commenttag;
}


void FilteringDataSource::setCommentTag( const string & x )
{
  _commenttag = x;
  if( x.empty() )
    _filtercomments = false;
}


