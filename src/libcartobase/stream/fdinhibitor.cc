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
#include <unistd.h>
#include <fcntl.h>
#include <cartobase/stream/fdinhibitor.h>
#include <cartobase/stream/fileutil.h>
#include <iostream>
#include <cerrno>

using namespace carto;
using namespace std;

fdinhibitor::fdinhibitor( int fd, bool permanent )
: _fd( fd ), _fdsave( 0 ), _fdblocker( 0 ), _permanent( permanent )
{}

fdinhibitor::fdinhibitor( FILE *fd, bool permanent )
:_fd( fileno(fd) ), _fdsave( 0 ), _fdblocker( 0 ), _permanent( permanent )
{}
  
fdinhibitor::ResetCallback::~ResetCallback()
{
}


map<string, fdinhibitor::ResetCallback *> & fdinhibitor::_callbacks()
{
  static map<string, ResetCallback *> _callbacks_map;
  return _callbacks_map;
}


fdinhibitor::~fdinhibitor()
{
  if( !_permanent )
    open();
}

void fdinhibitor::close(void)
{
  _fdsave = dup(_fd);
  _fdblocker = ::open( CARTOBASE_STREAM_NULLDEVICE, O_APPEND | O_WRONLY);
  dup2(_fdblocker, _fd);
}

void	fdinhibitor::open(void)
{
  if( _fdblocker == 0 )
    return; // not closed.

  dup2(_fdsave, _fd);
  ::close(_fdblocker);
  ::close(_fdsave);
  
  if( _fd == STDOUT_FILENO )
    std::cout.clear( std::ios_base::goodbit );
  else if( _fd == STDERR_FILENO )
    std::cerr.clear( std::ios_base::goodbit );
  else if( _fd == STDIN_FILENO )
    std::cin.clear( std::ios_base::goodbit );
  
   errno = 0;
  _fdblocker = 0;
  _fdsave = 0;

  notify( _fd );
}


void fdinhibitor::notify( int fd )
{
  map<string, ResetCallback *> & callbacks = _callbacks();
  map<string, ResetCallback *>::iterator i, e = callbacks.end();

  for( i=callbacks.begin(); i!=e; ++i )
    (*i->second)( fd );
}


void fdinhibitor::registerResetCallback( const std::string & name,
                                         ResetCallback *cbk )
{
  unregisterResetCallback( name );
  _callbacks()[name] = cbk;
}


bool fdinhibitor::hasResetCallback( const std::string & name )
{
  return _callbacks().find(name) != _callbacks().end();
}


void fdinhibitor::unregisterResetCallback( const std::string & name )
{
  map<string, ResetCallback *> & callbacks = _callbacks();
  map<string, ResetCallback *>::iterator i = callbacks.find( name );
  if( i != callbacks.end() )
  {
    callbacks.erase( i );
    delete i->second;
  }
}

