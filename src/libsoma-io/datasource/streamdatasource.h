#ifndef SOMAIO_DATASOURCE_STREAMDATASOURCE_H
#define SOMAIO_DATASOURCE_STREAMDATASOURCE_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                       // base class
//--- cartobase --------------------------------------------------------------
#include <cartobase/smart/rcptr.h>                           // member: _rcstr
//--- system -----------------------------------------------------------------
#include <iostream>
#include <string>
//----------------------------------------------------------------------------

namespace soma
{

  //==========================================================================
  //   S T R E A M
  //==========================================================================
  /// DataSource on a std::iostream (read/write stream)
  class StreamDataSource : public DataSource
  {
  public:
    /// Using an existing, external stream
    StreamDataSource( std::iostream &, 
                      const std::string & url = std::string() );
    /// Using a ref-counter to a stream
    StreamDataSource( carto::rc_ptr<std::iostream>, 
                      const std::string & url = std::string() );
    virtual ~StreamDataSource();
    std::iostream & stream();

    virtual std::string url() const;
    virtual DataSource* clone() const;
    virtual bool open( int mode );
    virtual bool allowsMemoryMapping() const;
    virtual int iterateMode() const;
    virtual bool isOpen() const;
    virtual bool eof() const;
    virtual bool operator ! () const;
    virtual void close();
    /// always returns 0
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char* data, unsigned long maxlen );
    virtual long writeBlock( const char* data, unsigned long len );
    virtual int getch();
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    bool isFile() const;

  private:
    std::iostream 		*_stream;
    carto::rc_ptr<std::iostream>	_rcstr;
    std::string			_url;
  };

  //==========================================================================
  //   I S T R E A M
  //==========================================================================
  /// DataSource on a std::istream (read-only stream)
  class IStreamDataSource : public DataSource
  {
  public:
    /// Using an existing, external stream
    IStreamDataSource( std::istream &, 
                       const std::string & url = std::string() );
    /// Using a ref-counter to a stream
    IStreamDataSource( carto::rc_ptr<std::istream>, 
                       const std::string & url = std::string() );
    virtual ~IStreamDataSource();
    std::istream & stream();

    virtual std::string url() const;
    virtual DataSource* clone() const;
    virtual bool open( int mode );
    virtual bool allowsMemoryMapping() const;
    virtual int iterateMode() const;
    virtual bool isOpen() const;
    virtual bool eof() const;
    virtual bool operator ! () const;
    virtual void close();
    /// always returns 0
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char* data, unsigned long maxlen );
    /// always fails
    virtual long writeBlock( const char* data, unsigned long len );
    virtual int getch();
    /// always fails
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    bool isFile() const;

  private:
    std::istream 		*_stream;
    carto::rc_ptr<std::istream>	_rcstr;
    std::string			_url;
  };

  //==========================================================================
  //   O S T R E A M
  //==========================================================================
  /// DataSource on a std::ostream (write-only stream)
  class OStreamDataSource : public DataSource
  {
  public:
    /// Using an existing, external stream
    OStreamDataSource( std::ostream &, 
                       const std::string & url = std::string() );
    /// Using a ref-counter to a stream
    OStreamDataSource( carto::rc_ptr<std::ostream>, 
                       const std::string & url = std::string() );
    virtual ~OStreamDataSource();
    std::ostream & stream();

    virtual std::string url() const;
    virtual DataSource* clone() const;
    virtual bool open( int mode );
    virtual bool allowsMemoryMapping() const;
    virtual int iterateMode() const;
    virtual bool isOpen() const;
    virtual bool eof() const;
    virtual bool operator ! () const;
    virtual void close();
    /// always returns 0
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    /// always fails
    virtual long readBlock( char* data, unsigned long maxlen );
    virtual long writeBlock( const char* data, unsigned long len );
    /// always fails
    virtual int getch();
    virtual int putch( int ch );
    /// always fails
    virtual bool ungetch( int ch );
    bool isFile() const;

  private:
    std::ostream 		*_stream;
    carto::rc_ptr<std::ostream>	_rcstr;
    std::string			_url;
  };
  
}
#endif
