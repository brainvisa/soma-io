#ifndef CARTOBASE_EXCEPTION_IOEXCEPT_H
#define CARTOBASE_EXCEPTION_IOEXCEPT_H

#include <cartobase/config/cartobase_config.h>
#include <stdexcept>
#include <string>

namespace carto
{

  /// general IO error (should be specialized)
  class io_error : public std::runtime_error
  {
  public:
    io_error( const std::string & arg, const std::string & filename = "" );
    virtual ~io_error() throw();
    virtual const char* what() const throw();
    std::string filename() const { return _filename; }

    static void launchErrnoExcept( const std::string & filename = "" );
    static void keepExceptionPriority( std::exception & e, int & prio, 
                                       int & type, std::string & message,
                                       int raiseprio = 0 );
    static void launchExcept( int code, const std::string & msg, 
                              const std::string & defmsg = "" );
    /// checks if errno is a disk full, access or quota error,
    /// then throw an exception if so.
    static void checkFatalIOErrno( const std::string & filename = "" );

  private:
    std::string			_filename;
    mutable  std::string	_formed_msg;
  };


  /// open failed (read or write), generally specialized
  class open_error : public io_error
  {
  public:
    open_error( const std::string & arg, const std::string & filename = "" );
    virtual ~open_error() throw();
  };


  /** file not found: trying to read an non-existing file, or write in a 
      non-existing directory */
  class file_not_found_error : public open_error
  {
  public:
    file_not_found_error( const std::string & arg, 
			  const std::string & filename );
    file_not_found_error( const std::string & filename = "" );
    virtual ~file_not_found_error() throw();
  };


  /// read or write permission denied
  class permission_error : public open_error
  {
  public:
    permission_error( const std::string & arg, const std::string & filename );
    permission_error( const std::string & filename = "" );
    virtual ~permission_error() throw();
  };


  /// open as a file something which is a directory
  class is_dir_error : public open_error
  {
  public:
    is_dir_error( const std::string & arg, const std::string & filename );
    is_dir_error( const std::string & filename = "" );
    virtual ~is_dir_error() throw();
  };


  /** write in a non-existing directory (more precise than 
      file_not_found_error ) */
  class no_dir_error : public file_not_found_error
  {
  public:
    no_dir_error( const std::string & arg, const std::string & filename );
    no_dir_error( const std::string & filename = "" );
    virtual ~no_dir_error() throw();
  };


  /// too many open files
  class too_many_descriptors_error : public open_error
  {
  public:
    too_many_descriptors_error( const std::string & arg, 
				const std::string & filename );
    too_many_descriptors_error( const std::string & filename = "" );
    virtual ~too_many_descriptors_error() throw();
  };


  /** read / write failed (bad medium, disk full, quota exceeded...).
      Normally specialized */
  class read_write_error : public io_error
  {
  public:
    read_write_error( const std::string & arg, 
		      const std::string & filename = "" );
    virtual ~read_write_error() throw();
  };


  /// disk / network failure
  class bad_medium_error : public read_write_error
  {
  public:
    bad_medium_error( const std::string & arg, const std::string & filename );
    bad_medium_error( const std::string & filename = "" );
    virtual ~bad_medium_error() throw();
  };


  /// disk full or quota exceeded (specialize for quota)
  class disk_full_error : public read_write_error
  {
  public:
    disk_full_error( const std::string & arg, const std::string & filename );
    disk_full_error( const std::string & filename = "" );
    virtual ~disk_full_error() throw();
  };


  /// quota exceeded
  class quota_exceeded_error : public disk_full_error
  {
  public:
    quota_exceeded_error( const std::string & arg, 
			  const std::string & filename );
    quota_exceeded_error( const std::string & filename = "" );
    virtual ~quota_exceeded_error() throw();
  };


  /// stream state flags errors
  class stream_state_error : public read_write_error
  {
  public:
    stream_state_error( const std::string & arg, 
			const std::string & filename = "" );
    virtual ~stream_state_error() throw();
  };


  /// EOF
  class eof_error : public stream_state_error
  {
  public:
    eof_error( const std::string & arg, const std::string & filename );
    eof_error( const std::string & filename = "" );
    virtual ~eof_error() throw();
  };


  /// stream not open
  class not_open_error : public stream_state_error
  {
  public:
    not_open_error( const std::string & arg, const std::string & filename );
    not_open_error( const std::string & filename = "" );
    virtual ~not_open_error() throw();
  };


  /// fail or bad bit is set
  class stream_fail_error : public stream_state_error
  {
  public:
    stream_fail_error( const std::string & arg, const std::string & filename );
    stream_fail_error( const std::string & filename = "" );
    virtual ~stream_fail_error() throw();
  };


  /// format-related errors (file parsing / interpreting)
  class format_error : public io_error
  {
  public:
    format_error( const std::string & arg, const std::string & filename );
    format_error( const std::string & filename = "" );
    virtual ~format_error() throw();
  };


  /// wrong format
  class wrong_format_error : public format_error
  {
  public:
    wrong_format_error( const std::string & arg, 
			const std::string & filename );
    wrong_format_error( const std::string & filename = "" );
    virtual ~wrong_format_error() throw();
  };


  /// format is recognized but something goes wrong
  class format_mismatch_error : public format_error
  {
  public:
    format_mismatch_error( const std::string & arg, 
			   const std::string & filename );
    format_mismatch_error( const std::string & filename = "" );
    virtual ~format_mismatch_error() throw();
  };


  /// corrupt data: not understood by current format
  class corrupt_stream_error : public format_mismatch_error
  {
  public:
    corrupt_stream_error( const std::string & arg, 
			  const std::string & filename );
    corrupt_stream_error( const std::string & filename = "" );
    virtual ~corrupt_stream_error() throw();
  };


  /** format is recognized but contains parameters that can not be handled 
      by current parser */
  class invalid_format_error : public format_mismatch_error
  {
  public:
    invalid_format_error( const std::string & arg, 
			  const std::string & filename );
    invalid_format_error( const std::string & filename = "" );
    virtual ~invalid_format_error() throw();
  };


  /** format is recognized but contains data types that can not be read 
      by current reader */
  class datatype_format_error : public invalid_format_error
  {
  public:
    datatype_format_error( const std::string & arg, 
			   const std::string & filename );
    datatype_format_error( const std::string & filename = "" );
    virtual ~datatype_format_error() throw();
  };


  /// parsing error
  class parse_error : public carto::format_error
  {
  public:
    /**	Only use this version or the next one
	@param what_arg what the parser was waiting for
	@param token_arg current token
	@param filename_arg name of the file
	@param line_arg line where the error occurred
    */
    parse_error( const std::string& what_arg,
                 const std::string& token_arg,
                 const std::string& filename_arg,
		 int line_arg );
    /**	Only use this version or the previous one
	@param what_arg what the parser was waiting for
	@param token_arg current token
	@param line_arg line where the error occurred
    */
    parse_error( const std::string& what_arg,
                 const std::string& token_arg,
                 int line_arg );
    /*	DO NOT USE! DEPRECIATED
	@param what_arg not used
	@param line_arg number of the line in the file
	@param filename_arg name of the file
    */
    parse_error( const std::string& what_arg, int line_arg,
                 const std::string& filename_arg = std::string() )
      __attribute__((__deprecated__));

    virtual ~parse_error() throw ();

    int line() const;

  private:
    static std::string init( const std::string& what_arg, 
                             const std::string& token_arg,
                             const std::string& filename_arg,
                             int line_arg );
    static std::string init( const std::string& what_arg, 
                             const std::string& token_arg,
                             int line_arg );

    const std::string _filename;
    const int _line;
  };


  class syntax_check_error : public invalid_format_error
  {
  public:
    syntax_check_error( const std::string & arg, 
			const std::string & filename );
    syntax_check_error( const std::string & filename = "" );
    virtual ~syntax_check_error() throw();
  };

}

#endif


