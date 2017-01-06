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
                                                                                
#ifndef CARTOBASE_STREAM_FDINHIBITOR_H
#define CARTOBASE_STREAM_FDINHIBITOR_H

#include <string>
#include <map>
#include <stdio.h>

namespace carto
{

  /** Temporarily closes a file descriptor (typically, stdout/stderr).
      Currently, this low-level implementation doesn't work with all C++ 
      streams: on some systems C++ streams stay in a bad state afterwards and 
      are silent forever. Standard IO streams (cin, cout, cerr) are checked 
      and protected against this problem.
   */
class fdinhibitor
{
public:
  class ResetCallback
  {
  public:
    virtual ~ResetCallback();
    virtual void operator ()( int fd ) = 0;
  };

  /** fd: file descriptor to inhibate, permanent: if false, don't reopen the
      stream when deleting the fdinhibitor */
  fdinhibitor( int fd, bool permanent=false )
    : _fd( fd ), _fdsave( 0 ), _fdblocker( 0 ), _permanent( permanent )
  {}
  /** fd: file to inhibate, permanent: if false, don't reopen the
      stream when deleting the fdinhibitor */
  fdinhibitor( FILE *fd, bool permanent=false )
    : _fd( fileno( fd ) ), _fdsave( 0 ), _fdblocker( 0 ),
      _permanent( permanent )
  {}
  ~fdinhibitor();

  void	close(void);
  void	open(void);
  void notify( int fd );

  static void registerResetCallback( const std::string & name,
                                     ResetCallback *cbk );
  static bool hasResetCallback( const std::string & name );
  static void unregisterResetCallback( const std::string & name );

private:
  static std::map<std::string, ResetCallback *> & _callbacks();

  int           _fd;
  int           _fdsave;
  int           _fdblocker;
#ifdef _WIN32
  std::string   _tmpfile;
#endif
  bool          _permanent;

};

}
#endif
