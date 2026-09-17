//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//--- system -----------------------------------------------------------------
#include <stdio.h>
//----------------------------------------------------------------------------

using namespace soma;
using namespace carto;
using namespace std;


string StreamUtil::readWhile( DataSource & ds, const string & s, 
                              bool ascii )
{
  string	res;

  if( !ascii )
    return res;

  unsigned	n = 0;
  if( !ds.isOpen() )
    return res;

  int c = ds.getch();

  ++n;
  while( c != EOF && s.find_first_of(c) != string::npos )
    {
      res += c;
      c = ds.getch();
      ++n;
    }
  if( c != EOF )
    {
      ds.ungetch( c );
      --n;
    }
  return res;
}


string StreamUtil::readUntil( DataSource & ds, const string & s, 
                              bool ascii )
{
  string	res;

  if( !ascii )
    return res;

  unsigned	n = 0;
  if( !ds.isOpen() )
    return res;

  int c = ds.getch();

  ++n;
  while( c != EOF && s.find_first_of(c) == string::npos )
    {
      res += c;
      c = ds.getch();
      ++n;
    }
  if( c != EOF )
    {
      ds.ungetch( c );
      --n;
    }
  return res;
}


bool StreamUtil::skip( DataSource & ds, const string & s, bool ascii )
{
  if( !ascii )
    return true;

  unsigned	n = 0;
  if( !ds.isOpen() )
    return false;

  int c = ds.getch();

  ++n;
  while( c != EOF && s.find_first_of(c) != string::npos )
    {
      c = ds.getch();
      ++n;
    }
  if( c != EOF )
    {
      ds.ungetch( c );
      --n;
    }
  return ds.isOpen();
}


bool StreamUtil::skipUntil( DataSource & ds, const string & s, 
                            bool ascii )
{
  if( !ascii )
    return true;

  unsigned	n = 0;
  if( !ds.isOpen() )
    return false;

  int c = ds.getch();

  ++n;
  while( c != EOF && s.find_first_of(c) == string::npos )
    {
      c = ds.getch();
      ++n;
    }
  if( c != EOF )
    {
      ds.ungetch( c );
      --n;
    }
  return ds.isOpen();
}


bool StreamUtil::getline( DataSource & ds, string & x )
{
  x.clear();
  int c = ds.getch();
  while( ds.isOpen() && c != '\n' && c != '\r' && c != 0 )
  {
    x += static_cast<char>( c );
    if( ds.eof() )
      break;
    c = ds.getch();
  }
  return ds.isOpen();
}


bool StreamUtil::skip( istream & is, const string & s, bool ascii )
{
  if( !ascii )
    return true;

  unsigned	n = 0;
  if( is.eof() )
    return false;

  int c = is.get();
  ++n;
  while( c != EOF && s.find_first_of(c) != std::string::npos )
    {
      c = is.get();
      ++n;
    }
  if( c != EOF )
    {
      is.unget();
      --n;
    }
  return !!is;
}
