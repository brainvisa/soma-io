
#include <cartobase/thread/cpu.h>
#include <cartobase/stream/fileutil.h>
#include <cartobase/stream/directory.h>
#if defined(_WIN32)
// for GetSystemInfo() on Windows
#include <windows.h>
#elif defined(__APPLE__)
// for MPProcessorsScheduled() on Mac OS X
#include <CoreServices/CoreServices.h>
#elif defined(__FreeBSD__)
// for sysctl() on FreeBSD
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(__OpenBSD__) || defined(__NetBSD__)
// for sysctl() on OpenBSD and netBSD
#include <sys/param.h>
#include <sys/sysctl.h>
#elif defined(__hpux)
// for pstat_getdynamic() on HP-UX
#include <sys/param.h>
#include <sys/pstat.h>
#else
// for sysconf() on Linux, Solaris, IRIX, and Tru64
#include <unistd.h>
#endif
#include <fstream>


using namespace carto;
using namespace std;


unsigned carto::cpuCount()
{
  static unsigned nCpu = 0;
  if( nCpu != 0 )
    return nCpu;

#if defined(_WIN32)
  // total number of processors, Windows
  SYSTEM_INFO sysinfo;
  GetSystemInfo( &sysinfo );
  nCpu = static_cast<int>( sysinfo.dwNumberOfProcessors );
#elif defined(__APPLE__)
  // number of processors online, Mac OS X
  nCpu = static_cast<int>( MPProcessorsScheduled() );
#elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
  // total number of processors, FreeBSD, OpenBSD, and NetBSD
  int mib[2];
  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  int nCpui = 1;
  size_t len = sizeof( nCpu );
  sysctl( mib, 2, &nCpui, &len, NULL, 0 );
  nCpu = static_cast<unsigned>( nCpui );
#elif defined(__hpux) 
  // number of processors online, HP-UX
  struct pst_dynamic psd; 
  pstat_getdynamic(&psd, sizeof(psd), 1, 0);
  nCpu = static_cast<unsigned>( psd.psd_proc_cnt );
#elif defined(__linux) || defined(__sun) || defined(__osf)
  // number of processors online, Linux, Solaris, and Tru64
  nCpu = static_cast<unsigned>( sysconf( _SC_NPROCESSORS_ONLN ) );
  // check quotas
  const Directory quotdir = Directory( "/etc/systemd/system/user-.slice.d" );
  if( quotdir.isValid() )
  {
    set<string> files = quotdir.files();
    set<string>::iterator i, e = files.end();
    char buffer[1000];
    for( i=files.begin(); i!=e; ++i )
    {
      if( i->length() < 6 || i->substr( i->length() - 5, 5 ) != ".conf" )
        continue;
      ifstream f( (quotdir.dirname() + FileUtil::separator() + *i ).c_str() );
      unsigned n = 0;
      while( f.good() && !f.eof() )
      {
        f.getline( buffer, 999 );
        ++n;
        if( n == 30 )
          break;
        string line(buffer);
        if( line.substr(0, 9) == "CPUQuota=" )
        {
          line = line.substr( 9, line.length() - 10 );
          double fcpu = atof( line.c_str() );
          if( fcpu / 100 < nCpu )
            nCpu = int( rint( fcpu / 100 ) );
          break;
        }
      }
    }
  }
#elif defined(__sgi)
  // number of processors online, IRIX
  nCpu = static_cast<unsigned>( sysconf( _SC_NPROC_ONLN ) );
#else
  nCpu = 1;
#endif
  return nCpu;
}


