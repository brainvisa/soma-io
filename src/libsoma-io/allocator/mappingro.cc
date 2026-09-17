//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/allocator/mappingro.h>                  // class declaration
#include <soma-io/datasource/filedatasource.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/exception/assert.h>
//--- system -----------------------------------------------------------------
#include <iostream>
#include <errno.h>
#ifndef _WIN32
  #include <sys/types.h>                                                // sys
  #include <sys/stat.h>                                                 // sys
  #include <sys/mman.h>                                                 // sys
  #include <unistd.h>
  #include <fcntl.h>
#endif
//----------------------------------------------------------------------------
// #define _ALLOCATOR_VERBOSE_

using namespace soma;
using namespace carto;
using namespace std;

MappingROAllocator*& MappingROAllocator::_allocptr()

{
  static MappingROAllocator    *allocptr = 0;

  return allocptr;

}



MappingROAllocator::~MappingROAllocator()
{
  _allocptr() = 0;
}


char* MappingROAllocator::allocate( size_t n, size_t sz, DataSource* ds ) const
{
#ifdef _ALLOCATOR_VERBOSE_
  cout << "MappingROAllocator::allocate n: " << n << ", sz: " << sz 
       << ", DS: " << ( ds ? ds->url() : "<none>" ) << endl;
#endif

  FileDataSource	*fds = dynamic_cast<FileDataSource *>( ds );
  string		name = ds->url();
  offset_t		offset = 0;
  if( fds )
    offset = fds->initialOffset();

  map< std::string, map<soma::offset_t, char*> >::iterator 
    it2 = _mapName2Ptr.find( name );
  if( it2 != _mapName2Ptr.end() )
    {
      map<soma::offset_t, char*>::iterator 
        it3 = it2->second.find( offset );
      if( it3 != it2->second.end() )
        {
          ++_mapRef[ it3->second ];
#ifdef _ALLOCATOR_VERBOSE_
          cout << singleton() << endl;
#endif
          return it3->second;
        }
    }

  if( name.length() )
    {
#ifdef _WIN32

      HANDLE 
        filedest = CreateFile( name.c_str(), GENERIC_READ, 
                               FILE_SHARE_READ, 0, OPEN_EXISTING, 
                               FILE_ATTRIBUTE_READONLY, 0 );
      if( filedest != INVALID_HANDLE_VALUE )
        {
          HANDLE	fmap = CreateFileMapping( filedest, 0, PAGE_READONLY, 
                                                  0, 0, 0 );
          if( fmap != INVALID_HANDLE_VALUE )
            {
              char	*buffer = static_cast<char*>(
                MapViewOfFile( fmap, FILE_MAP_READ, offset >> 32,
                               offset & 0xffffffff, n * sz ) );
              if( buffer )
                {
                  _mapName2Ptr[ name ][ offset ] = buffer;
                  _mapPtr2Name[ buffer ] = _FileId( name, offset );
                  WinHandles	& wh = _mapDesc[ buffer ];
                  wh.fileH = filedest;
                  wh.mapH = fmap;
                  _mapRef[ buffer ] = 1;
#ifdef _ALLOCATOR_VERBOSE_
                  cout << singleton() << endl;
#endif
                  return buffer;
                }
              CloseHandle( fmap );
            }
          CloseHandle( filedest );
        }
      else 
        cerr << "unable to open " << name << " !" << endl;    

#else	// _WIN32

      int fildest = ::open( name.c_str(), O_RDONLY, 0 );
      if( fildest != -1 )
        {
          char* buffer = static_cast<char *>(
                           mmap( 0, n * sz, PROT_READ, MAP_SHARED,
                                 fildest, offset ) );
          if( buffer != MAP_FAILED )
            {
              _mapName2Ptr[ name ][ offset ] = buffer;
              _mapPtr2Name[ buffer ] = _FileId( name, offset );
              _mapDesc[ buffer ] = fildest;
              _mapRef[ buffer ] = 1;
#ifdef _ALLOCATOR_VERBOSE_
              cout << singleton() << endl;
#endif
              return buffer;
            }
          cerr << "unable to use memory mapping\n";
          if( errno == ENOMEM )
            cerr << "Not enough memory left (or addressable)" << endl;
          if(fds)
            fds->close();
          return 0;
        }
      cerr << "unable to open " << name << " !" << endl;    

#endif	// _WIN32
      return 0;
    }
  return 0;
}


void MappingROAllocator::deallocate( 
#ifdef _WIN32
char* ptr, size_t, size_t 
#else
char* ptr, size_t n, size_t sz
#endif
) const
{
  if ( ptr )
    {
      map<char*, _FileId>::iterator it = _mapPtr2Name.find( ptr );
      ASSERT( it != _mapPtr2Name.end() );
      string name = it->second.first;
      soma::offset_t off = it->second.second;

      if( name.length() )
	{
	  int count = -- _mapRef[ ptr ];
	  if ( count == 0 )
	    {
#ifdef _WIN32

              UnmapViewOfFile( ptr );
              WinHandles	& wh = _mapDesc[ ptr ];
              CloseHandle( wh.mapH );
              CloseHandle( wh.fileH );

#else	// _WIN32

	      munmap( ptr, n * sz );
	      close( _mapDesc[ ptr ] );

#endif	// _WIN32

              map<string, map<soma::offset_t, char*> >::iterator 
                ip = _mapName2Ptr.find( name );
              ip->second.erase( off );
              if( ip->second.empty() )
                _mapName2Ptr.erase( ip );
	      _mapPtr2Name.erase( ptr );
	      _mapDesc.erase( ptr );
	      _mapRef.erase( ptr );
	    }
#ifdef _ALLOCATOR_VERBOSE_
	  cout << singleton() << endl;
#endif
	}
    }
}


const MappingROAllocator & MappingROAllocator::singleton()
{
  if( !_allocptr() )
    _allocptr() = new MappingROAllocator;
  return *_allocptr();
}


std::ostream& soma::operator << ( ostream& os, 
                                   const MappingROAllocator & thing )
{
  os << "{ MAP_RO, this=" << &thing << ", ";
  map< char*, MappingROAllocator::_FileId>::const_iterator it;
  for ( it = thing._mapPtr2Name.begin(); it!= thing._mapPtr2Name.end(); 
	++it )
    {
      os << "{ " << it->first  << ", ( " 
         << it->second.first << ", " << it->second.second << " ), ";
#ifndef _WIN32
      os << thing._mapDesc[ it->first ] << ", ";
#endif
      os << thing._mapRef[ it->first ] << " }, ";
    }
  os << "NULL }" << flush;

  return os;
}
