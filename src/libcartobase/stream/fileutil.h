/* This software and supporting documentation are distributed by
 *     Institut Federatif de Recherche 49
 *     CEA/NeuroSpin, Batiment 145,
 *     91191 Gif-sur-Yvette cedex
 *     France
 *
 * This software is governed by the CeCILL-B license under
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the
 * terms of the CeCILL-B license as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-B license and that you accept its terms.
 */

#ifndef CARTOBASE_STREAM_FILEUTIL_H
#define CARTOBASE_STREAM_FILEUTIL_H

#include <cartobase/config/cartobase_config.h>
#include <string>
#include <list>


#ifdef _WIN32
#define CARTOBASE_STREAM_NULLDEVICE "nul"
#else
#define CARTOBASE_STREAM_NULLDEVICE "/dev/null"
#endif

namespace carto
{
  class Object;

  /// Files and filenames portable utilities
  class FileUtil
  {
  public:
    //==========================================================================
    //   P A T H S
    //==========================================================================
    /// returns '/' on Unix systems and '\' on Windows systems
    static char separator();
    /// character separating paths in a list (generally in an environment 
    /// variable).
    /// \return ':' on Unix systems and ';' on Windows systems */
    static char pathSeparator();
    /// this is NOT conform to basename() from libgen:
    /// ::basename( "/" ) = "/", carto::FileUtil::basename( "/" ) = "", 
    /// ::basename( "/usr/" ) = "usr", 
    /// carto::FileUtil::basename( "/usr/" ) = ""
    /// ::basename( "." ) = ".", carto::FileUtil::basename( "." ) = "", 
    /// ::basename( ".." ) = "..", carto::FileUtil::basename( ".." ) = "", 
    ///
    /// But I think this fits better the use we may have of it...
    static std::string basename( const std::string & );
    /// this is NOT conform to dirname() from libgen:
    /// ::dirname( "/usr/" ) = "/", 
    /// carto::FileUtil::dirname( "/usr/" ) = "/usr"
    ///
    /// But I think this fits better the use we may have of it...
    static std::string dirname( const std::string & );
    /// relative path of fname, relatively to base
    static std::string relpath( const std::string &fname,
                                const std::string & base );
    /// Checks file existence, type and read/write permission. 
    /// \return string code containing "+" if the file exists, "r" if it is 
    /// readable, "w" if it is writeable, "d" if it is a directory, 
    /// "x" if it is executable (so "+rw" is the normal code for a RW file)
    static std::string fileStat( const std::string & );
    /// returns the file extension (without the dot)
    static std::string extension( const std::string & );
    /// returns the filename without its extension
    static std::string removeExtension( const std::string & );
    /// true if path is an absolute path, false if it is relative
    static bool isAbsPath( const std::string & path );
    /// make an absolute path
    static std::string absPath( const std::string & path );
    /// opens a temporary file.
    /// \param prefix is used to name the temporary file. If it is a local file
    /// (with no path), the temporary directory will be prefixed to it, 
    /// otherwise the specified directory is used. A (random) generated number 
    /// will be suffixed to the prefix filename, and the extension (if any) 
    /// will also be appended to it. So "toto.ima" can become 
    /// "/tmp/toto123456.ima".
    /// \param fd will be filled with the file descriptor of the temp file
    /// \return the full name of the temporary file
    ///
    /// On Unix, the created file doesn't have to be deleted, it is 
    /// hidden (already unlinked).
    ///
    /// On Windows, I guess the file has to be manually deleted...
    static std::string temporaryFile( const std::string & prefix, int & fd );
    /// Creates a temporary directory
    static std::string temporaryDirectory( const std::string & prefix );
    /// interpret the filenames string as a list of filenames and split it.
    /// escape characters and quotes (single and double) are allowed.
    static std::list<std::string>
        filenamesSplit( const std::string & filenames,
                        const std::string & separator );

    /// Replaces slashes with backslashes characters
    static std::string windowsFilename(const std::string & filename);

    /// Replaces backslashes with slashes characters on Windows OS
    /// on other OS, leaves filename unchanged
    static std::string linuxFilename(const std::string & filename);
    
    //==========================================================================
    //   U R I
    //==========================================================================
    /// It is possible to treat an URI (Uniform Resource Identifier) instead of
    /// a simple path. It would have the following syntax : 
    /// "/my/path/file.ext?options1=this&option2=that"

    /// Extracts the classical filename  from the URI :
    /// "/my/path/file.ext"
    static std::string uriFilename( const std::string & filein );
    /// Extracts the options from the URI and returns them as a dictionary
    /// of strings :
    /// ( "options1" => "this", "option2" => "that" )
    static Object uriOptions( const std::string & filein );
    /// Extracts the options from the URI and returns them as a string :
    /// "?options1=this&option2=that"
    static std::string uriCopyOptions( const std::string & filein );

  };

}

#endif
