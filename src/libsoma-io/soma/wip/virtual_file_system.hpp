// Virtual layer on the top of GIO. To be discussed.

#ifndef SOMA_VIRTUAL_FILE_SYSTEM
#define SOMA_VIRTUAL_FILE_SYSTEM

#include <string>
#include <list>

#include <gio/gio.h>

#include <soma/rcptr.hpp>


namespace soma
{

typedef std::string UTF8String;
typedef GCancellable *CancellableRef;

class FileInputStream : public RCObject
{
public:
  // GInputStream
  virtual std::size_t read( void *buffer,
                            const std::size_t count,
                            CancellableRef cancellable ) = 0;
  virtual std::size_t read_all( void *buffer,
                                const std::size_t count,
                                CancellableRef cancellable ) = 0;
  virtual std::size_t skip( const std::size_t count,
                            CancellableRef cancellable ) = 0;
  virtual bool close( CancellableRef cancellable ) = 0;
  virtual bool is_closed() const = 0;

  // GSeekable
  virtual std::diff_t tell() const = 0;
  virtual bool can_seek() const = 0;
  virtual bool seek( std::diff_t offset,
                     GSeekType type,
                     CancellableRef cancellable ) = 0;
};

typedef rcptr< FileInputStream > FileInputStreamRef;

class FileOutputStream
{
public:
  // GFileOutputStream
  virtual std::string get_etag() const = 0;
  
  // GOutputStream
  virtual std::size_t write( void *buffer,
                            const std::size_t count,
                            CancellableRef cancellable ) = 0;
  virtual std::size_t write_all( void *buffer,
                                 const std::size_t count,
                                 CancellableRef cancellable ) = 0;
  virtual std::size splice( FileInputStreamRef source,
                            CancellableRef cancellable ) = 0;
  virtual std::size flush( CancellableRef cancellable ) = 0;
  virtual bool close( CancellableRef cancellable ) = 0;
  virtual bool is_closed() const = 0;

  // GSeekable
  virtual std::diff_t tell() const = 0;
  virtual bool can_seek() const = 0;
  virtual bool seek( std::diff_t offset,
                     GSeekType type,
                     CancellableRef cancellable ) = 0;
  virtual bool can_truncate() const = 0;
  virtual bool truncate( std::diff_t offset,
                         CancellableRef cancellable ) = 0;
};

typedef rcptr< FileOutputStream > FileOutputStreamRef;



class File
{
public:

  typedef rcptr< File > FileRef;
  typedef rcptr< std::list< FileRef > > FileEnumeratorRef;
  typedef rcptr< PropertySet > PropertySetRef;
  typedef rcptr< std::list< GFileAttributeInfo > > FileAttributeInfoListRef;
  typedef GFileProgressCallbackRef *FileProgressCallbackRef;
  
  virtual FileRef dup() const = 0;
  virtual uint32_t hash() const = 0;
  virtual bool equal( const FileRef &other ) const = 0;
  virtual std::string get_base_name() const = 0;
  virtual std::string get_path() const = 0;
  virtual UTF8String get_uri() const = 0;
  virtual UTF8String get_parse_name() const = 0;
  virtual FileRef get_parent() const = 0;
  virtual FileRef get_child( const std::string &name ) const = 0;
  virtual FileRef get_child_for_display_name( const UTF8String &display_name ) const = 0;
  virtual bool has_prefix( const FileRef &prefix ) const = 0;
  virtual std::string get_relative_path( const FileRef &parent ) const = 0;
  virtual FileRef resolve_relative_path( const std::string & ) const = 0;
  virtual bool is_native() const = 0;
  virtual bool has_uri_scheme( cosnt string &uri_scheme ) const = 0;
  virtual std::string get_uri_scheme() const = 0;
  virtual FileInputStreamRef read( CancellableRef cancellable ) const = 0;
  virtual FileOutputStream append_to( GFileCreateFlags flags,
                                      CancellableRef cancellable ) const = 0;
  virtual FileOutputStream create( GFileCreateFlags flags,
                                   CancellableRef cancellable ) const = 0;
  virtual FileOutputStream replace( const std::string etag,
                                    bool make_backup,
                                    FileCreateFlags,
                                    CancellableRef cancellable ) const = 0;
  virtual PropertySetRef query_info( const std::string &attributes, 
                                     GFileQueryInfoFlags flags, 
                                     CancellableRef cancellable ) const = 0;
  virtual bool query_exists( CancellableRef cancellable ) const = 0;
  virtual GFileType query_file_type( GFileQueryInfoFlags flags, 
                                     CancellableRef cancellable ) const = 0;
  virtual PropertySetRef query_filesystem_info( const std::string &attributes, 
                                                CancellableRef cancellable ) const = 0;
  virtual FileEnumeratorRef enumerate_children( const std::string &attributes,
                                                GFileQueryInfoFlags flags,
                                                CancellableRef cancellable ) const = 0;
  virtual FileRef set_display_name( const UTF8String &display_name,
                                    CancellableRef cancellable ) const = 0;
  virtual bool file_delete( CancellableRef cancellable ) const = 0;
  virtual bool copy( const FileRef &destination,
                     GFileCopyFlags flags,
                     CancellableRef cancellable,
                     FileProgressCallbackRef progress_callback ) const = 0;
  virtual bool move( const FileRef &destination,
                     GFileCopyFlags flags,
                     CancellableRef cancellable,
                     FileProgressCallbackRef progress_callback ) const = 0;
  virtual bool make_directory( CancellableRef cancellable ) const = 0;
  virtual bool make_directory_with_parents( CancellableRef cancellable ) const = 0;
  virtual bool make_symbolic_link( const std::string &symlink_value,
                                   CancellableRef cancellable ) const = 0;
  virtual FileAttributeInfoListRef query_settable_attributes( CancellableRef cancellable ) const = 0;
  virtual FileAttributeInfoListRef query_writable_namespaces( CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const UTF8String &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const std::string &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const uint32_t &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const int32_t &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const uint64_t &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool set_attribute( const std::string &attribute,
                              const int64_t &,
                              const GFileQueryInfoFlags flags,
                              CancellableRef cancellable ) const = 0;
  virtual bool copy_attributes( const FileRef &destination,
                                GFileCopyFlags flags,
                                CancellableRef cancellable ) const = 0;
};

} // namespace soma

#endif // ifndef  SOMA_VIRTUAL_FILE_SYSTEM
