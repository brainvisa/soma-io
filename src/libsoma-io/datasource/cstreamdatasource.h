#ifndef SOMAIO_DATASOURCE_CSTREAMDATASOURCE_H
#define SOMAIO_DATASOURCE_CSTREAMDATASOURCE_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                       // base class
//--- system -----------------------------------------------------------------
#include <cstdio>
//----------------------------------------------------------------------------

namespace soma
{

  /// C stream data source: base class for all sources based on a FILE*
  /// (stdio streams)
  class CStreamDataSource : public DataSource
  {
  public:
    CStreamDataSource( FILE *f = 0, int mode = Read );
    virtual ~CStreamDataSource();

    virtual DataSource* clone() const;
    virtual int iterateMode() const;
    virtual bool isOpen() const;
    /// useless for a file descriptor: a fd is already open
    virtual bool open ( int mode );
    virtual void close();
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char* data, unsigned long maxlen );
    virtual long writeBlock( const char* data, unsigned long len );
    virtual int getch();
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    /// in the general case, memory mapping is not possible
    virtual bool allowsMemoryMapping() const;

    FILE *filePtr() const;	// should it be non-const ?
    void setFilePtr( FILE *f );

  protected:
    FILE	*_f;
  };

}

#endif
