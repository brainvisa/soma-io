#ifndef SOMAIO_DATASOURCE_FILEDATASOURCE_H
#define SOMAIO_DATASOURCE_FILEDATASOURCE_H

#ifndef _WIN32
  #define CARTO_FILEDS_AS_FD
#endif
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#ifdef CARTO_FILEDS_AS_FD
  #include <soma-io/datasource/fddatasource.h>                   // base class
#else
  #include <soma-io/datasource/cstreamdatasource.h>              // base class
#endif
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

namespace soma
{

  class FileDataSource : 
    #ifdef CARTO_FILEDS_AS_FD
      public FDDataSource
    #else
      public CStreamDataSource
    #endif
  {
  public:
    FileDataSource( const std::string & filename, offset_t offset = 0, 
                    int mode = Read );
    virtual ~FileDataSource();
    virtual std::string url() const;
    offset_t initialOffset() const;

    virtual DataSource* clone() const;
    virtual bool open( int mode );
    virtual bool allowsMemoryMapping() const;
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );

  private:
    std::string _url;
    offset_t    _offset;
  };

}

#endif
