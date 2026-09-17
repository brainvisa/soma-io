#ifndef SOMAIO_IO_FORMATDICTIONARY_H
#define SOMAIO_IO_FORMATDICTIONARY_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/object/object.h>
//--- system -----------------------------------------------------------------
#include <string>
#include <map>
#include <set>
#include <vector>
//----------------------------------------------------------------------------

namespace soma
{
  template <typename T> class FormatReader;
  template <typename T> class FormatWriter;

  /// \brief Formats holder for each format of Cartograph object, used by
  /// Reader and Writer
  ///
  /// FormatDictionary is a plug-in mechanism to allow new formats to
  /// be registered.\n
  /// It uses at lower level specific IO classes for each format (GisFormat
  /// for GIS, SpmReader for SPM, etc., and IO classes for
  /// other object types (meshes, buckets, textures, ...))
  ///
  /// \par Adding new IO formats
  ///
  /// The standard carto/aims libraries include a set of already compiled
  /// Readers and writers.
  ///
  /// If you need to use other types, you'll have to include
  /// \c soma-io/io/reader_d.h and / or \c soma-io/io/writer_d.h in order
  /// to have the template functions definitions, \c soma-io/io/reader.h and
  /// \c soma-io/io/writer.h are only the class declarations.
  /// In addition, you may want to specialize the registerBaseFormats()
  /// functions of specific FormatDictionary specializations.
  ///
  /// \see Reader Writer DataSourceInfo Process
  template<typename T>
  class FormatDictionary
  {
  public:
    static void registerFormat( const std::string & formatID, 
                                FormatReader<T>* format,
                                const std::vector<std::string> & extensions );
    static void registerFormat( const std::string & formatID, 
                                FormatWriter<T>* format,
                                const std::vector<std::string> & extensions );
    static FormatReader<T>* readFormat( const std::string & format );
    static FormatWriter<T>* writeFormat( const std::string & format );
    static const std::multimap<std::string, std::string> & readExtensions();
    static const std::multimap<std::string, std::string> & writeExtensions();
    static std::set<std::string> readFormats();
    static std::set<std::string> writeFormats();
    /// builds base formats maps.
    /// The formats list is dynamically extensible, you can register your own
    /// whenever you want
    static void registerBaseFormats();
    static void init();

  private:
    static std::map<std::string, FormatReader<T>*>  & _readformats();
    static std::map<std::string, FormatWriter<T>*>  & _writeformats();
    static std::multimap<std::string, std::string>  & _readextensions();
    static std::multimap<std::string, std::string>  & _writeextensions();
  };


  /// Global dictionary of supported object types for IO
  class IOObjectTypesDictionary
  {
  public:
    typedef std::set<std::string> (*FormatInfo)();
    static void registerReadType( const std::string & datatype, 
                                  FormatInfo info );
    static void registerWriteType( const std::string & datatype, 
                                   FormatInfo info );
    static bool hasReadType( const std::string & datatype );
    static bool hasWriteType( const std::string & datatype );
    static std::map<std::string, FormatInfo> & readTypes();
    static std::map<std::string, FormatInfo> & writeTypes();
  };

  extern template class FormatDictionary<carto::GenericObject>;
  extern template class FormatDictionary<carto::Object>;
}

#endif
