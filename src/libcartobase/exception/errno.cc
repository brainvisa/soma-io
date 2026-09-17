//=============================================================================
//	H E A D E R  F I L E S
//=============================================================================

#include <cartobase/exception/errno.h>
#include <errno.h>
#include <string.h>


//=============================================================================
//	M E T H O D S
//=============================================================================


using namespace std;
using namespace carto;

errno_error::errno_error()
            : runtime_error( init() ),
              _errno_err( errno )
{
}


errno_error::errno_error( const string& what_arg )
	    : runtime_error( what_arg ),
              _errno_err( errno )
{
}


errno_error::~errno_error() throw ()
{
}


int errno_error::thrown_errno() const
{

  return _errno_err;

}


string errno_error::init()
{

  return string( strerror( errno ) );

}
