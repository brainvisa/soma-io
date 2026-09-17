#ifndef SOMAIO_DATASOURCE_BUFFERDATASOURCE_H
#define SOMAIO_DATASOURCE_BUFFERDATASOURCE_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>                       // base class
//----------------------------------------------------------------------------

namespace soma
{

  class BufferDataSource : public DataSource
  {
  public:
    BufferDataSource( char* ptr = 0, unsigned long size = 0, int mode = Read );
    virtual ~BufferDataSource();

    virtual DataSource* clone() const;
    virtual int iterateMode() const;
    virtual bool isOpen() const;
    virtual bool open ( int mode );
    virtual void close();
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char * data, unsigned long maxlen );
    virtual long writeBlock( const char * data, unsigned long len );
    virtual int getch();
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    virtual bool allowsMemoryMapping() const;

    const char *buffer() const;
    char *buffer();
    void setBuffer( char* ptr, unsigned long size, int mode );

  private:
    char		*_buffer;
    unsigned long	_size;
    unsigned long	_pos;
  };

}

#endif
