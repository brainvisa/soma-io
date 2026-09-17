#ifndef SOMAIO_IMAGE_GISIMAGEREADER_H
#define SOMAIO_IMAGE_GISIMAGEREADER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagereader.h>                             // heritage
#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/reader/itemreader.h>                      // read + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageReader for GIS files.
  template<typename T>
  class GisImageReader : public ImageReader<T>, protected ChainDataSource
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      GisImageReader();
      virtual ~GisImageReader();

      //======================================================================
      //   I M A G E R E A D E R
      //======================================================================
      virtual void read( T * dest, DataSourceInfo & dsi,
                         std::vector<int> & pos,
                         std::vector<int> & size,
                         std::vector<long> & stride,
                         carto::Object options = carto::none() );

      virtual void updateParams( DataSourceInfo & dsi );
      virtual void resetParams();
      virtual ImageReader<T>* cloneReader() const;
      virtual std::string formatID() const { return "GIS"; }

      virtual bool open( DataSourceInfo & dsi );
      virtual void close( DataSourceInfo & dsi );
      virtual bool isOpen( const DataSourceInfo & dsi ) const;

    protected:
      //======================================================================
      //   D A T A S O U R C E
      //======================================================================
      virtual DataSource* clone() const;
      virtual int iterateMode() const;
      virtual offset_t size() const;
      virtual offset_t at() const;
      virtual bool at( offset_t pos );
      virtual long readBlock( char * data, unsigned long maxlen );
      virtual long writeBlock( const char * data, unsigned long len );
      virtual int getch();
      virtual int putch( int ch );
      virtual bool ungetch( int ch );
      virtual bool allowsMemoryMapping() const;
      virtual bool setpos( const std::vector<int> & pos );

    protected:
      long readStridedBlock( char * data, unsigned long len, long stride = 1 );

      std::unique_ptr<ItemReader<T> > _itemr;
      std::vector<std::vector<int> >  _sizes;
      bool  _binary;
      bool  _byteswap;
  };

}

#endif
