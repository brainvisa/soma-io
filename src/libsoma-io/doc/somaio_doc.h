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

// This file is only here for Doxygen (doc generation tool)
// Nothing to do with the code, in fact...

namespace soma
{

/*! \mainpage Soma-IO documentation

  \anchor somaio

  Soma-IO: IO system.


  \section somaio_provides Provided libraries and commands:

  - soma-io library


  \section somaio_requirements Requirements:

  The soma-io library now needs the following libraries installed on the
  compiling system:
  - libsigc++ 2.0 or newer: http://sourceforge.net/projects/libsigc/
  - libxml2: http://xmlsoft.org/

  Optionally, the followng libraries may be used if available:
  - boost: http://boost.org for thread-safe reference counters

  - it also relies on the \ref cartobase library, which is part of the same package.


  \section somaio_building Building:

  Use <a href="http://www.cmake.org/">CMake</a>, a cross-platform and open-source build system. More information on <a href="http://brainvisa.info/brainvisa-cmake/compile_existing.html">The BrainVISA-cmake build system</a>. See also <a href="http://brainvisa.info/web/infrastructure.html#sources">this page on BrainVISA website</a>.


  \section somaio_maincontents Main contents:

  - namespace: ::soma
  - commandline options parsing system:
    \link carto::OptionsParser OptionsParser\endlink and \link carto::CartoApplication CartoApplication\endlink
  - <b>\ref soma_io</b>:
    \link soma::DataSource DataSource\endlink,
    \link soma::DataSourceList DataSourceList\endlink,
    \link soma::Reader Reader\endlink, \link soma::Writer Writer\endlink,
    \link soma::DataSourceInfo DataSourceInfo\endlink
    \link soma::ReaderAlgorithm ReaderAlgorithm\endlink
  - <b>\ref allocators</b>
  - STL-like containers:
    - vectors using extended allocators: 
      \link soma::AllocatedVector AllocatedVector\endlink
  - plugins system: \link carto::Plugin Plugin\endlink, 
    \link carto::PluginManager PluginManager\endlink, 
    \link carto::PluginLoader PluginLoader\endlink
  - other general utility classes, functions and macros:
    - libraries information printing: \link carto::Info Info\endlink


  \section somaio_programming Other programming rules and notes:

  - \ref porting

  - <b><a href="../aimsdata/html/en/changelog.html">Change Log</a></b>

*/



/*! \page allocators Allocators system

  Allocators are handled by programmers via the AllocatorContext class. 
  This class holds information on how to allocate a (large) buffer. It is 
  prepared before the actual allocation, and at that time only a decision 
  will be made on how to allocate the buffer, depending on its memory size, 
  the available memory, the binary compatibility with a file to read...

  Allocation contexts holds the following information:

  - a DataSource: file, buffer, or none. This data source represents both the
    file to read from and the reader system needed to get and convert data 
    to the internal memory format of the system. Alternately the data source 
    can be an already allocated memory buffer, or no source at all (which 
    will result in allocating an uninitialized buffer)
  - a status: allocated or unallocated
  - when determined, an actual LowLevelAllocator will be used. Currently
    supported methods include:
    - Memory allocation
    - Direct, read-only memory mapping
    - Read/write memory mapping with copy of the whole DataSource
    - Direct read/write memory mapping (will modify the DataSource)
  - an access mode (read only, read/write, or internal modification): see 
    AllocatorStrategy::DataAccess

  As the allocator context has to check dynamically the amount of free RAM 
  and swap memory, and make a decision according to them each time it has to 
  allocate data, one allocation is rather slow. So this allocator system 
  should be reserved to large data blocks, and avoided to allocate many small 
  objects.

  If many small objects have to be allocated anyway, the allocation mode 
  may be forced by directly providing a LowLevelAllocator (generally the 
  MemoryAllocator::singleton()) to the allocator context.

  Allocation context may be also used in the IO system (\ref reader_writer) to request (or try to) specific allocation modes, such as memory mapping.
*/

}


