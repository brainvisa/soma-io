#ifndef SOMAIO_WRITER_ITEMWRITER_H
#define SOMAIO_WRITER_ITEMWRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasource/datasource.h>
#include <soma-io/utilities/asciidatasourcetraits.h>
//----------------------------------------------------------------------------
#include <iostream>

namespace soma
{

  ///	Low-level "small item" writer, used by higher-level file writers. 
	/// \sa ItemWriter
  template<typename T>
  class ItemWriter
  {
  public:
    virtual ~ItemWriter() {}

    /// Factory function
    virtual ItemWriter<T>* writer( bool binary = true, 
				   bool bswap = false ) const = 0;

    bool write( DataSource & ds, const T & item ) const 
    { return write( ds, &item, 1 ) == 1; }
    virtual long write( DataSource & ds, const T* pitem, 
			size_t n ) const = 0;
  };


  ///	Default low-levels writers.
	/// They are a default implementation of the writers for most common types,
	/// and can be specialized for other types if needed.
	/// We have already specialized AimsVector writers
	/// \sa DefaultItemReader 
  template <typename T>
  class DefaultItemWriter : public ItemWriter<T>
  {
  public:
    DefaultItemWriter() : ItemWriter<T>() { }
    virtual ~DefaultItemWriter() { }

    virtual long write( DataSource & ds, const T* pitem, size_t n ) const;
    virtual ItemWriter<T>* writer( bool binary = true, 
				   bool bswap = false ) const;
  };


  template <typename T>
  class DefaultAsciiItemWriter : public DefaultItemWriter<T>
  {
  public:
    DefaultAsciiItemWriter() : DefaultItemWriter<T>() { }
    virtual ~DefaultAsciiItemWriter() { }

    virtual long write( DataSource & ds, const T* pitem, size_t n ) const;
  };


  template <typename T>
  class DefaultBSwapItemWriter : public DefaultItemWriter<T>
  {
  public:
    DefaultBSwapItemWriter() : DefaultItemWriter<T>() { }
    virtual ~DefaultBSwapItemWriter() { }

    virtual long write( DataSource & ds, const T* pitem, size_t n ) const;
  };


  template<typename T> ItemWriter<T>* 
  DefaultItemWriter<T>::writer( bool binary, 
				bool bswap ) const
  {
    if( !binary )
      return( new DefaultAsciiItemWriter<T> );
    if( bswap )
      return( new DefaultBSwapItemWriter<T> );
    return( new DefaultItemWriter<T> );
  }


  template <typename T> inline long
  DefaultItemWriter<T>::write( DataSource & ds, const T* pitem, 
			       size_t n ) const
  {
    long len = ds.writeBlock( (const char *) pitem, sizeof(T) * n );
    return len / sizeof( T );
  }


  template <typename T> inline
  long DefaultAsciiItemWriter<T>::write( DataSource & ds, const T* pitem, 
					 size_t n ) const
  {
    const T* ptr = pitem;
    size_t	i;
    for( i=0; i<n && ds.isOpen(); ++i, ++ptr )
      ds << *ptr << " ";
    return (long) i;
  }


  template <typename T> inline
  long DefaultBSwapItemWriter<T>::write( DataSource & ds, const T* pitem, 
					 size_t n ) const
  {
    std::vector<uint8_t> pd( n * sizeof(T) );
    const uint8_t	*ps = (uint8_t *) pitem;
    for( size_t k=0; k<n*sizeof(T); k+=sizeof(T) ) {
      for( size_t b=0; b<sizeof(T)/2; ++b ) {
        pd[k+b] = ps[k+sizeof(T)-1-b];
        pd[k+sizeof(T)-1-b] = ps[k+b];
      }
    }
    return ds.writeBlock( (const char *) &pd[0], sizeof(T) * n ) / sizeof(T);
  }


  //	specializations

  template <> inline long 
  DefaultItemWriter<Void>::write( DataSource &, const Void*, size_t n ) const
  {
    return (long) n;
  }


  template <> inline long 
  DefaultAsciiItemWriter<Void>::write( DataSource &, const Void*, 
				       size_t n ) const
  {
    return (long) n;
  }


  template <> inline long 
  DefaultBSwapItemWriter<Void>::write( DataSource &, const Void*, 
				       size_t n ) const
  {
    return (long) n;
  }

}


#endif
