//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/allocator/mappingrw.h>                  // class declaration
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

using namespace soma;
using namespace carto;
using namespace std;

MappingRWAllocator *& MappingRWAllocator::_allocptr()

{
  static MappingRWAllocator    *allocptr = 0;

  return allocptr;

}



MappingRWAllocator::~MappingRWAllocator()
{
  _allocptr() = 0;
}


char* MappingRWAllocator::allocate( size_t n, size_t sz, DataSource* ds ) const
{
  FileDataSource	*fds = dynamic_cast<FileDataSource *>( ds );
  string		name = fds->url();
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

  if ( name.length() )
    {
#ifdef _WIN32

      HANDLE 
        filedest = CreateFile( name.c_str(), GENERIC_READ | GENERIC_WRITE, 
                               0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 
                               0 );
      if( filedest != INVALID_HANDLE_VALUE )
        {
          HANDLE	fmap = CreateFileMapping( filedest, 0, PAGE_READWRITE, 
                                                  0, 0, 0 );
          if( fmap != INVALID_HANDLE_VALUE )
            {
              char	*buffer = 
                (char *) MapViewOfFile( fmap, FILE_MAP_WRITE, offset >> 32, 
                                        offset & 0xffffffff, n * sz );
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

#else // _WIN32

      int fildest = ::open( name.c_str(), O_RDWR, 0644 );
      if ( fildest != -1 )
        {
          char* buffer = static_cast<char *>(
                           mmap( 0, n * sz, PROT_READ | PROT_WRITE,
                                 MAP_SHARED, fildest, offset ) );
          if ( buffer != MAP_FAILED )
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
          close( fildest );
          cerr << "could not mmap (RW) " << name << endl;
          if( errno == ENOMEM )
            cerr << "Not enough memory left (or addressable)" << endl;
          return 0;
        }
      cerr << "unable to open " << name << " !" << endl;

#endif	// _WIN32
      return 0;
    }
  return 0; 
}


void MappingRWAllocator::deallocate( 
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

      if ( name.length() )
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


const MappingRWAllocator & MappingRWAllocator::singleton()
{
  if( !_allocptr() )
    _allocptr() = new MappingRWAllocator;
  return *_allocptr();
}


std::ostream& soma::operator << ( ostream& os, 
                                   const MappingRWAllocator & thing )
{
  os << "{ MAP_RW, this=" << &thing << ", ";
  std::map< char*, MappingRWAllocator::_FileId>::const_iterator it;
  for ( it = thing._mapPtr2Name.begin(); it!= thing._mapPtr2Name.end(); 
	it++ )
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
