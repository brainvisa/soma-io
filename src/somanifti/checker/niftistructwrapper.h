#ifndef SOMAIO_CHECKER_NIFTISTRUCTWRAPPER_H
#define SOMAIO_CHECKER_NIFTISTRUCTWRAPPER_H

#include <cartobase/object/object.h>
#include <cartobase/smart/rcptr.h>
#include <soma-io/nifticlib/niftilib/nifti2_io.h>

namespace soma
{

  /** This class holds the nifti_image structure of nifticlib, and frees it
      when it is deleted. It can be set in a generic object so as to fit in a
      header.
   */
  class NiftiStructWrapper : public carto::RCObject
  {
  public:
    NiftiStructWrapper( nifti_image* nim=0 ) : nim( nim ) {}
    virtual ~NiftiStructWrapper();
    nifti_image* nim;
  };

  /** This class holds the nifti_image stream (znzFile), and frees it
      when it is deleted.
   */
  class NiftiFileWrapper : public carto::RCObject
  {
  public:
    NiftiFileWrapper( znzFile znz ) : znzfile( znz ) {}
    virtual ~NiftiFileWrapper();
    znzFile znzfile;
  };

}

namespace carto
{
  DECLARE_GENERIC_OBJECT_TYPE( carto::rc_ptr<soma::NiftiStructWrapper> )
}

#endif

