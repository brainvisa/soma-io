//=============================================================================
//        H E A D E R  F I L E S
//=============================================================================

#include <cartobase/object/sreader.h>
#include <cartobase/object/lexicon.h>
#include <cartobase/exception/file.h>
#include <cartobase/exception/parse.h>

// DEBUG FIXME
#include <iostream>

using namespace carto;
using namespace std;

//=============================================================================
//        M E T H O D S
//=============================================================================

SyntaxReader::SyntaxReader(const string& filename)
        : _filename(filename), _stream(filename.c_str())
{
  // are files opened?
  if (!_stream)
    throw file_error(_filename);
}


SyntaxReader::~SyntaxReader()
{
}


void
SyntaxReader::skipWhile(const string& s)
{
  if( _stream.eof() )
    return;
  int c = _stream.get();
  while( !_stream.eof() && c != EOF
    && s.find_first_of(c) != string::npos )
  {
    c = _stream.get();
  }
  _stream.unget();
}


string
SyntaxReader::readUntil(const string& s)
{
  string token;

  if( _stream.eof() )
    return token;
  int c = _stream.get();
  while( !_stream.eof() && c != EOF
    && s.find_first_of(c) == string::npos )
  {
    token += static_cast<char>(c);
    c = _stream.get();
  }
  _stream.unget();

  return token;
}


string
SyntaxReader::nextToken()
{
  skipWhile(" \t\n");
  return readUntil(" \t\n");
}

void
SyntaxReader::read(SyntaxSet& rules)
{
  string token;

  while (1)
  {
    token = nextToken();
    if( token == "" &&
        ( _stream.eof()
          // these strange conditions happen in gcc 5.2
          || ( !_stream.good() && !_stream.bad() ) ) )
      break;
    if (token != Lexicon::begin())
      throw parse_error(Lexicon::begin(), token, \
        _filename, _stream.line());

    token = nextToken();
    if( _stream.eof() || !_stream.good() || token != Lexicon::syntax() )
      throw parse_error(Lexicon::syntax(), token, \
        _filename, _stream.line());

    token = nextToken();
    if( _stream.eof() || !_stream.good() || rules.find(token) != rules.end() )
      throw parse_error("{new syntactic attribute}", \
        token, _filename, _stream.line());

    read(rules[token]);
  }
}


void
SyntaxReader::read(Syntax& syntax)
{
  string token;

  while (1)
  {
    token = nextToken();

    if (_stream.eof())
      throw parse_error(Lexicon::end(), token, \
        _filename, _stream.line());
    if (token == Lexicon::end())
      break;
    string name = token;

    token = nextToken();
    if( _stream.eof()
        // these strange conditions happen in gcc 5.2
        || ( !_stream.good() && !_stream.bad() ) )
      throw parse_error("{semantic attribute type}", token, \
        _filename, _stream.line());
    string type = token;

    skipWhile(" \t");
    token = readUntil(" \t\n");
    if( _stream.eof() || !_stream.good() )
      throw parse_error("!", token, \
        _filename, _stream.line());
    bool needed = (token == "") ? false : true;

    Semantic semantic;
    semantic.type = type;
    semantic.needed = needed;
    syntax[name] = semantic;
  }
}


//=============================================================================
//        F U N C T I O N
//=============================================================================

SyntaxReader&
operator>>(SyntaxReader& reader, SyntaxSet& rules)
{
  reader.read(rules);
  return reader;
}
