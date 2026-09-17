#ifndef CARTOBASE_CONFIG_PATHS_H
#define CARTOBASE_CONFIG_PATHS_H

#include <string>
#include <list>

namespace carto
{

  /** Paths to configuration / shared data directories

  The functions in Paths provide a portable access to the paths of
  useful directories that may depend on the site or user configuration, or on
  environment variables (such as \c BRAINVISA_SHARE or \c HOME ).

  Path is implemented as a namespace rather than a class with static methods
  so that it can easily be extended in upper-level libraries.
  */
  namespace Paths
  {
    /// User home directory
    const std::string& home();
    /// Temporary directory used for general usage
    const std::string& tempDir();
    /** Temporary directory used for memory mapping with copy - this directory
        has better be large
    */
    const std::string& memmap();
    /// Root path of the BrainVisa install directory
    const std::string & installRoot();
    /// The base shared directory for all cartograph / SHFJ data
    const std::string& globalShared();
    /** Shared directory for SHFJ-specific data. Depending on the
        installation and configuration, this may be the same as the
        globalShared() directory.
    */
    const std::string& shfjShared();
    /** Shared resource files search path.
        It typically includes:
        - home() + "/.brainvisa";
        - home();
        - globalShared()
    */
    const std::list<std::string> & resourceSearchPath();
    void addResourceSearchPath( const std::string & path,
                                bool atbeginning=false );
    /** Find a file in the shared resources directories, or in a personal
        directory.
        Files are looked in the search paths given by resourceSearchPath().
        For each search path, the filename is looked in a subdirectory
        containing the project name and version (if not specified, cartobase
        version is used), and then in a subdirectory containing the project
        name without version. For personal directories, a dot may be prepended
        to the project name for use as a hidden directory (classical on Unix),
        like "/home/user/.anatomist/..."
        The function stops at the first match and return the first path.
    */
    std::string findResourceFile( const std::string & filename,
                                  const std::string & project = "",
                                  const std::string & version = "" );
    /** Same as findResourceFile() except that it looks for every occurrence
        of the searched file (or directory).
    */
    std::list<std::string> findResourceFiles( const std::string & filename,
                                              const std::string & project = "",
                                              const std::string & version = ""
                                            );
    /** argv[0], ie executable name and path; only available when a
        CartoApplication has been built */
    const std::string &argv0();
    void setArgv0( const std::string & argv0 );
  }

}


#endif


