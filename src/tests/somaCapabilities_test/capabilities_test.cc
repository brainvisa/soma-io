//--- soma-io ------------------------------------------------------------------
#include <soma-io/config/soma_config.h>
#include <soma-io/datasourceinfo/datasourcecapabilities.h>
//--- cartobase ----------------------------------------------------------------
#include <cartobase/exception/assert.h>
//------------------------------------------------------------------------------

using namespace soma;
using namespace carto;

int main()
{
  DataSourceCapabilities dc;
  ASSERT( !dc.isInit()              &&
          !dc.allowsMemoryMapping() &&
          !dc.isThreadSafe()        &&
          !dc.isOrdered()           &&
          !dc.canSeekVoxel()        &&
          !dc.canSeekLine()         &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setMemoryMapping( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          !dc.isThreadSafe()        &&
          !dc.isOrdered()           &&
          !dc.canSeekVoxel()        &&
          !dc.canSeekLine()         &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setThreadSafe( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          !dc.isOrdered()           &&
          !dc.canSeekVoxel()        &&
          !dc.canSeekLine()         &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setOrdered( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          dc.isOrdered()            &&
          !dc.canSeekVoxel()        &&
          !dc.canSeekLine()         &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setSeekVoxel( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          dc.isOrdered()            &&
          dc.canSeekVoxel()         &&
          !dc.canSeekLine()         &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setSeekLine( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          dc.isOrdered()            &&
          dc.canSeekVoxel()         &&
          dc.canSeekLine()          &&
          !dc.canSeekSlice()        &&
          !dc.canSeekVolume()       );

  dc.setSeekSlice( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          dc.isOrdered()            &&
          dc.canSeekVoxel()         &&
          dc.canSeekLine()          &&
          dc.canSeekSlice()         &&
          !dc.canSeekVolume()       );

  dc.setSeekVolume( true );
  ASSERT( dc.isInit()               &&
          dc.allowsMemoryMapping()  &&
          dc.isThreadSafe()         &&
          dc.isOrdered()            &&
          dc.canSeekVoxel()         &&
          dc.canSeekLine()          &&
          dc.canSeekSlice()         &&
          dc.canSeekVolume()        );
}
