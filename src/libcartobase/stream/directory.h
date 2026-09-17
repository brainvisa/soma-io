#ifndef CARTOBASE_STREAM_DIRECTORY_H
#define CARTOBASE_STREAM_DIRECTORY_H

#include <cartobase/config/cartobase_config.h>
#include <string>
#include <set>

namespace carto
{
  /**	Encapsulation of a portable opendir / readdir version
   */
  class Directory
  {
  public:
    Directory( const std::string & dname );
    ~Directory();
    std::set<std::string> files() const;
    std::set<std::string> directories() const;
    void chdir( const std::string & dname );
    std::string dirname() const;
    bool isValid() const;
    void mkdir() const;
    /// recursive mkdir()
    void makedirs() const;

  private:
    struct Private;
    Private	*d;
  };

}

#endif
