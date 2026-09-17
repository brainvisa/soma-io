#ifndef SOMAIO_ALLOCATOR_MAPPINGRW_H
#define SOMAIO_ALLOCATOR_MAPPINGRW_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/allocator/allocator.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/cartobase_config.h>
//--- system -----------------------------------------------------------------
#include <map>
#ifdef _WIN32
  #include <windows.h>
#endif
//----------------------------------------------------------------------------

namespace soma
{

  class MappingRWAllocator : public LowLevelAllocator
  {
  public:
    virtual ~MappingRWAllocator();

    virtual char* allocate( size_t n, size_t sz, DataSource* ds ) const;
    virtual void deallocate( char* ptr, size_t n, size_t sz ) const;

    bool canDuplicate() const { return false; }

    static const MappingRWAllocator & singleton();

    friend std::ostream& operator << ( std::ostream& os, 
				       const MappingRWAllocator & thing );

  private:
    typedef std::pair<std::string, offset_t> _FileId;

    MappingRWAllocator() : LowLevelAllocator()
    { _allocptr() = 0; }

    static MappingRWAllocator *& _allocptr();
    mutable std::map<char*, _FileId>	_mapPtr2Name;
    mutable std::map<std::string, std::map<offset_t, char*> > 
    _mapName2Ptr;
#ifdef _WIN32
    struct WinHandles
    {
      HANDLE	fileH;
      HANDLE	mapH;
    };
    mutable std::map<char*, WinHandles>		_mapDesc;
#else
    mutable std::map<char*, int>		_mapDesc;
#endif
    mutable std::map<char*, int>		_mapRef;
  };

  std::ostream& operator << ( std::ostream& os, 
			      const MappingRWAllocator & thing );

}

#endif
