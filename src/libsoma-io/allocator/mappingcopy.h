#ifndef SOMAIO_ALLOCATOR_MAPPINGCOPY_H
#define SOMAIO_ALLOCATOR_MAPPINGCOPY_H
//--- soma-io ----------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/allocator/allocator.h>
//--- cartobase --------------------------------------------------------------
#include <cartobase/config/cartobase_config.h>
//--- system -----------------------------------------------------------------
#include <string>
#include <map>
#ifdef _WIN32
  #include <windows.h>
#endif
//----------------------------------------------------------------------------

namespace soma
{

  class MappingCopyAllocator : public LowLevelAllocator
  {
  public:
    virtual ~MappingCopyAllocator();

    virtual char *allocate( size_t n, size_t elemsize, 
                            DataSource* datasource ) const;
    virtual void deallocate( char* ptr, size_t n, size_t sz ) const;

    static const MappingCopyAllocator & singleton();

    friend std::ostream& operator << ( std::ostream&, 
				       const MappingCopyAllocator & );

  private:
    MappingCopyAllocator() : LowLevelAllocator() { _allocptr() = 0; }

    static MappingCopyAllocator			*&_allocptr();
    #ifdef _WIN32
      mutable std::map<char*, HANDLE>		_mapDesc;
    #else
      mutable std::map<char*, std::string>	_mapname;
      mutable std::map<char*, int>		      _mapDesc;
    #endif

  };

  std::ostream& operator << ( std::ostream& os, 
			      const MappingCopyAllocator & thing );

}


#endif
