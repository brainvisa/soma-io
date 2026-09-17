#ifndef SOMAIO_DATASOURCE_FILTERINGDATASOURCE_H
#define SOMAIO_DATASOURCE_FILTERINGDATASOURCE_H
//--- somaio -----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/chaindatasource.h>                  // base class
//--- system -----------------------------------------------------------------
#include <string>
//----------------------------------------------------------------------------

namespace soma
{

  /// Filtering data source: filters out comments and/or counts lines in a
  /// ASCII stream
  class FilteringDataSource : public ChainDataSource
  {
  public:
    FilteringDataSource( carto::rc_ptr<DataSource> in, int line_num = 1 );
    virtual ~FilteringDataSource();

    virtual DataSource* clone() const;
    virtual int iterateMode() const;
    virtual offset_t size() const;
    virtual offset_t at() const;
    virtual bool at( offset_t pos );
    virtual long readBlock( char* data, unsigned long maxlen );
    virtual long writeBlock( const char* data, unsigned long len );
    virtual int getch();
    virtual int putch( int ch );
    virtual bool ungetch( int ch );
    virtual bool allowsMemoryMapping() const;

    bool filtersComments() const;
    bool countsLines() const;
    void setFiltersComments( bool );
    void setCountsLines( bool );
    int lineNumber() const;
    void setLineNumber( int );
    std::string commentTag() const;
    void setCommentTag( const std::string & );

  protected:
    bool	_filtercomments;
    bool	_countlines;
    int		_line;
    std::string	_commenttag;
  };

}

#endif
