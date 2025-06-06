################################################################################
## libCartobase

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.in" "${CMAKE_BINARY_DIR}/include/cartobase/config/version.h" @ONLY )
install( FILES "${CMAKE_BINARY_DIR}/include/cartobase/config/version.h" 
                   DESTINATION "include/cartobase/config" 
                   COMPONENT ${PROJECT_NAME}-dev )
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in" "${CMAKE_BINARY_DIR}/include/cartobase/config/config.h" @ONLY )
install( FILES "${CMAKE_BINARY_DIR}/include/cartobase/config/config.h"
                   DESTINATION "include/cartobase/config"
                   COMPONENT ${PROJECT_NAME}-dev )

## still in cartobase
set( _headers
   algorithm/algorithm.h
   config/cartobase_config.h
   config/paths.h
   config/verbose.h
   containers/block.h
   containers/nditerator.h
   doc/cartobase_doc.h
   doc/object_doc.h
   exception/assert.h
   exception/errno.h
   exception/file.h
   exception/format.h
   exception/ioexcept.h
   exception/parse.h
   object/attributed.h
   object/headered.h
   object/lexicon.h
   object/object.h
   object/object_d.h
   object/object_factory.h
   object/object_internal.h
   object/property.h
   object/propertyfilter.h
   object/sprinter.h
   object/sreader.h
   object/swriter.h
   object/syntax.h
   object/syntobject.h
   object/syntobject_d.h
   pattern/observable.h
   pattern/observer.h
   sigc++/cartosigcmacro.h
   sigc++/cartosigcmacroend.h
   sigc++/sigc++.h
   sigc++/signal.h
   sigc++/trackable.h
   smart/mutexrcptr.h
   smart/rcobject.h
   smart/rcptr.h
   smart/rcptrtrick.h
   smart/refwrapper.h
   smart/scopedptr.h
   smart/sharedptr.h
   smart/smartdoc.h
   smart/weakobject.h
   smart/weakptr.h
   stream/attachable_cuifstream.h
   stream/counter.h
   stream/cuifstream.h
   stream/directory.h
   stream/fdinhibitor.h
   stream/fdstream.h
   stream/fileutil.h
   stream/fistreambuf.h
   stream/fostreambuf.h
   stream/sstream.h
   stream/stringutil.h
   stream/uncomment.h
   thread/cpu.h
   thread/exclusiveContext.h
   thread/loopContext.h
   thread/loopThread.h
   thread/mutex.h
   thread/mutexprivatepthread.h
   thread/mutexprivatewindows.h
   thread/semaphore.h
   thread/threadCancel.h
   thread/threadCondition.h
   thread/threadedLoop.h
   thread/threadGauge.h
   thread/thread_g.h
   thread/thread.h
   type/byte_order.h
   type/conversion.h
   type/converter_d.h
   type/converter.h
   type/datatypetraits.h
   type/datatypeinfo.h
   type/limits.h
   type/limits_gcc3.h
   type/select.h
   type/string_conversion.h
   type/types.h
   type/typetraits.h
   type/voxelvalue.h
   type/voxelvalue_d.h
   type/voxelrgb.h
   type/voxelrgb_decl.h
   type/voxelrgb_def.h
   type/voxelrgba.h
   type/voxelrgba_decl.h
   type/voxelrgba_def.h
   type/voxelhsv.h
   uuid/uuid.h
   zoolib/atomiccount.h
   zoolib/ZAtomic.h
   zoolib/zconfig.h
   zoolib/zconfigd.h
   zoolib/zconfigtest.h
   zoolib/ZDebug.h
)

## moved to soma-io
set( _headers
  "${_headers}"
   allocator/allocator.h
   allocator/mappingcopy.h
   allocator/mappingro.h
   allocator/mappingrw.h
   config/info.h
   containers/allocatedvector.h
   datasource/bufferdatasource.h
   datasource/chaindatasource.h
   datasource/cstreamdatasource.h
   datasource/datasource.h
   datasource/fddatasource.h
   datasource/filedatasource.h
   datasource/filteringdatasource.h
   datasource/streamdatasource.h
   getopt/getopt.h
   object/achecker.h
   object/areader.h
   object/awriter.h
   object/pythonreader.h
   object/pythonreader_d.h
   object/pythonwriter.h
   object/pythonwriter_d.h
   io/asciidatasourcetraits.h
   io/creator.h
   io/datasourceinfo.h
   io/formatchecker.h
   io/formatdictionary.h
   io/formatdictionary_d.h
   io/formatreader.h
   io/formatreader_d.h
   io/formatwriter.h
   io/headertranslator.h
   io/itemreader.h
   io/itemwriter.h
   io/minfXML2.h
   io/pythonformatchecker.h
   io/pythonformatreader.h
   io/pythonformatwriter.h
   io/reader.h
   io/reader_d.h
   io/readeralgorithm.h
   io/writer.h
   io/writer_d.h
   io/xmlformatchecker.h
   io/xmlformatreader.h
   plugin/plugin.h
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers cartobase headersTarget )

## still in cartobase
set( _cartobase_0_sources
  algorithm/algorithm.cc 
  config/paths.cc 
  config/verbose.cc 
  config/version.cc 
  exception/assert.cc 
  exception/errno.cc 
  exception/file.cc 
  exception/format.cc 
  exception/ioexcept.cc 
  object/headered.cc 
  object/object.cc 
  object/object_usual.cc 
  object/property.cc 
  object/propertyfilter.cc 
  object/sprinter.cc 
  object/sreader.cc 
  object/swriter.cc 
  object/syntobject.cc 
  pattern/observer.cc 
  smart/mutexrcptr.cc 
  smart/rcobject.cc 
  smart/sharedptr.cc 
  smart/weakobject.cc 
  stream/attachable_cuifstream.cc 
  stream/counter.cc 
  stream/cuifstream.cc 
  stream/directory.cc 
  stream/fdinhibitor.cc 
  stream/fdstream.cc 
  stream/fileutil.cc 
  stream/fistreambuf.cc 
  stream/fostreambuf.cc 
  stream/stringutil.cc
  stream/uncomment.cc
  thread/cpu.cc 
  thread/exclusiveContext.cc 
  thread/loopContext.cc 
  thread/loopThread.cc 
  thread/mutex.cc 
  thread/semaphore.cc 
  thread/threadCancel.cc 
  thread/threadCondition.cc 
  thread/threadedLoop.cc 
  thread/threadGauge.cc 
  thread/thread.cc 
  type/byte_order.cc 
  type/converter.cc 
  type/datatypetraits.cc
  type/string_conversion.cc
  type/voxelvalue.cc
  uuid/uuid.cc 
)

## still in cartobase
set( _cartobase_1_sources
  object/object_map.cc
  object/object_vector.cc
)

## still in cartobase
set( _cartobase_sources
  object/object_factory.cc 
  object/object_map_string_vector.cc 
  object/object_rcptr_map.cc 
  object/object_rcptr_map_string_vector.cc 
  object/object_set.cc 
)

add_library( cartobase_0 SHARED ${_cartobase_0_sources} )
add_dependencies( cartobase_0 ${headersTarget} )
set_property( TARGET cartobase_0 PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( cartobase_0 ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES}  )
if( WIN32 AND CMAKE_CROSSCOMPILING )
  target_link_libraries( cartobase_0 ws2_32 ole32 )
endif( WIN32 AND CMAKE_CROSSCOMPILING )
if( WIN32 )
  target_link_libraries( cartobase_0 wsock32 )
endif( WIN32 )
if(UNIX)
  target_link_libraries( cartobase_0 "m" "pthread" )
endif(UNIX)
if(APPLE)
  target_link_libraries( cartobase_0 "-framework CoreServices" )
endif(APPLE)
install( TARGETS cartobase_0
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

add_library( cartobase_1 SHARED ${_cartobase_1_sources} )
set_property( TARGET cartobase_1 PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( cartobase_1 "cartobase_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
if(UNIX)
  target_link_libraries( cartobase_1 "pthread" )
endif(UNIX)
install( TARGETS cartobase_1
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

add_library( cartobase SHARED ${_cartobase_sources} )
set_property( TARGET cartobase PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( cartobase "cartobase_1" "cartobase_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
if(UNIX)
  target_link_libraries( cartobase "pthread" )
endif(UNIX)
install( TARGETS cartobase
                   DESTINATION lib
                   COMPONENT ${PROJECT_NAME} )

IF ( DOXYGEN_FOUND )
  SET(component_name "cartobase")
  set( DOXYFILE_PREDEFINED "${SOMA-IO_DEFINITIONS}")
  set( _doxyheaders ${_headers} "config/config.h" ) # add config.h file
  BRAINVISA_GENERATE_DOXYGEN_DOC(
    _doxyheaders
    INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/${component_name}"
    COMPONENT "${component_name}" )
ENDIF ( DOXYGEN_FOUND )
