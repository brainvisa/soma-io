#ifndef SOMAIO_IMAGE_GISIMAGEWRITER_H
#define SOMAIO_IMAGE_GISIMAGEWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/image/imagewriter.h>                             // heritage
#include <soma-io/datasource/chaindatasource.h>                    // heritage
#include <soma-io/writer/itemwriter.h>                     // write + byteswap
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>                        // header, options
//--- system -----------------------------------------------------------------
#include <memory>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSourceInfo;

  /// ImageWriter for GIS files.
  template<typename T>
  class GisImageWriter : public ImageWriter<T>, protected ChainDataSource
  {
    public:
      //======================================================================
      //   C O N S T R U C T O R S
      //======================================================================
      GisImageWriter();
      virtual ~GisImageWriter();

      //======================================================================
      //   I M A G E R E A D E R
      //======================================================================
      virtual void write( const T * dest, DataSourceInfo & dsi,
        const std::vector<int> & pos,  /* taille 4 : x,y,z,t */
        const std::vector<int> & size, /* taille 4 : x,y,z,t */
        const std::vector<long> & strides,
        carto::Object options = carto::none() );

      virtual DataSourceInfo writeHeader( DataSourceInfo & dsi, 
                                          const T * source,
                                          const std::vector<int> & pos,
                                          const std::vector<int> & size,
                                          const std::vector<long> & strides,
                                          carto::Object options
                                            = carto::none() );

      virtual void updateParams( DataSourceInfo & dsi );
      virtual void resetParams();

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
      void buildDSList( DataSourceList & dsl, carto::Object options ) const;
      long writeStridedBlock( const char * data, unsigned long len,
                              long stride = 1 );

      std::unique_ptr<ItemWriter<T> > _itemw;
      std::vector<std::vector<int> >  _sizes;
      bool  _binary;
      bool  _byteswap;
  };

}

#endif
