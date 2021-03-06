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
