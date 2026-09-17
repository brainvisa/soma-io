#ifndef SOMAIO_DATASOURCE_FDDATASOURCE_H
#define SOMAIO_DATASOURCE_FDDATASOURCE_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                       // base class
//----------------------------------------------------------------------------

namespace soma
{

  /// File Descriptor data source: base class for all sources based on a
  /// file descriptor
  class FDDataSource : public DataSource
  {
  public:
    FDDataSource( int fd = -1, int mode = Read );
    virtual ~FDDataSource();

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

    int descriptor() const;	// should it be non-const ?
    void setDescriptor( int fd );
    bool isFile() const;

  protected:
    int _fd;
  };

}

#endif
