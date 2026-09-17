#ifndef SOMAIO_IO_WRITER_H
#define SOMAIO_IO_WRITER_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//----------------------------------------------------------------------------

namespace soma
{
  class DataSource;
  class DataSourceInfo;

  class GenericWriter
  {
  public:
    GenericWriter();
    GenericWriter( carto::rc_ptr<DataSource> ds );
    GenericWriter( const std::string & filename );
    GenericWriter( std::ostream & stream );
    virtual ~GenericWriter();

    ///	\brief Finds the correct format and writes the object
    ///
    /// Options can be passed to specify some writing parameters. The most
    /// important is the format (\c "format" property): if it is specified,
    /// this format is tried first, so you can use it to force the format,
    /// otherwise it will be determined from the filename extension (if
    /// available). If no extension and no format are given, the first
    /// working format will be used.
    template <typename T>
    bool write( const T & obj, carto::Object options = carto::none() );

    virtual std::string writtenObjectType() const = 0;

    const carto::rc_ptr<DataSource> dataSource() const;
          carto::rc_ptr<DataSource> dataSource();
    void attach( carto::rc_ptr<DataSource> ds );
    void attach( const std::string & filename );
    void attach( std::ostream & stream );

    /// flush the writing DataSource (if still open)
    void flush();
    /// close the writing DataSource
    void close();

  protected:
    carto::rc_ptr<DataSourceInfo>  _datasourceinfo;
  };


  // Specialize write<Object> to be the same as write<GenericObject>
  template <> inline bool GenericWriter::write( const carto::Object & obj,
                                                carto::Object options )
  {
    return write( *obj, options );
  }


  /// Generic writer for *every* format of Aims object.
  /// The Writer classes are built on the same model as the Reader classes
  /// and share the same FormatDictionary with them to store all
  /// known file formats.
  ///
  /// See \ref soma_io general explanations on how to use Reader and
  /// Writer classes.
  ///
  /// \see FormatDictionary Reader
  template<class T> class Writer : public GenericWriter
  {
  public:
    /* Note: constructors used to be inline, but this triggered a
       compiler/linker bug on MacOS 10.5, so we moved them to non-inline.
    */
    Writer();
    Writer( carto::rc_ptr<DataSource> ds );
    Writer( const std::string& filename );
    Writer( std::ostream & stream );
    virtual ~Writer();

    ///	\brief Finds the correct format and writes the object
    ///
    /// Options can be passed to specify some writing parameters. The most
    /// important is the format (\c "format" property): if it is specified,
    /// this format is tried first, so you can use it to force the format,
    /// otherwise it will be determined from the filename extension (if
    /// available). If no extension and no format are given, the first
    /// working format will be used.
    /// It is possible to specify which passes to process through
    /// \c passbegin and \c passend.
    /// - pass 1 : format hint
    /// - pass 2 : extension
    /// - pass 3 : empty extension
    /// - pass 4 : all writers
    virtual bool write( const T & obj,
                        carto::Object options = carto::none(),
                        int passbegin = 1 , int passend = 4 );

    virtual std::string writtenObjectType() const;
  };


  // Specialize Writer<Object> to be the same as Writer<GenericObject>
  template <> inline bool Writer<carto::Object>::write(
    const carto::Object & obj, carto::Object options,
    int passbegin, int passend )
  {
    Writer<carto::GenericObject> w( dataSource() );
    return w.write( *obj, options, passbegin, passend );
  }

  extern template class Writer<carto::GenericObject>;
  extern template class Writer<carto::Object>;
}

template <class T>
inline soma::GenericWriter &
operator << ( soma::GenericWriter & writer, const T & thing )
{
  writer.write( thing );
  return writer;
}


template <class T>
inline soma::Writer<T> &
operator << ( soma::Writer<T> & writer, const T & thing )
{
  writer.write( thing );
  return writer;
}


#endif
