configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.in" "${CMAKE_BINARY_DIR}/include/cartobase/config/version.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/cartobase/config/version.h" 
                   DESTINATION "include/cartobase/config" 
                   COMPONENT ${PROJECT_NAME}-dev )
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in" "${CMAKE_BINARY_DIR}/include/cartobase/config/config.h" @ONLY )
BRAINVISA_INSTALL( FILES "${CMAKE_BINARY_DIR}/include/cartobase/config/config.h"
                   DESTINATION "include/cartobase/config"
                   COMPONENT ${PROJECT_NAME}-dev )

set( _headers
   algorithm/algorithm.h
   allocator/allocator.h
   allocator/mappingcopy.h
   allocator/mappingro.h
   allocator/mappingrw.h
   config/cartobase_config.h
   config/info.h
   config/paths.h
   config/verbose.h
   containers/allocatedvector.h
   containers/block.h
   datasource/bufferdatasource.h
   datasource/chaindatasource.h
   datasource/cstreamdatasource.h
   datasource/datasource.h
   datasource/fddatasource.h
   datasource/filedatasource.h
   datasource/filteringdatasource.h
   datasource/streamdatasource.h
   doc/cartobase_doc.h
   doc/getopt_doc.h
   doc/io_doc.h
   doc/object_doc.h
   exception/assert.h
   exception/errno.h
   exception/file.h
   exception/format.h
   exception/ioexcept.h
   exception/parse.h
   getopt/getopt.h
   object/achecker.h
   object/areader.h
   object/attributed.h
   object/awriter.h
   object/headered.h
   object/lexicon.h
   object/object.h
   object/object_d.h
   object/object_factory.h
   object/object_internal.h
   object/property.h
   object/propertyfilter.h
   object/pythonreader.h
   object/pythonreader_d.h
   object/pythonwriter.h
   object/pythonwriter_d.h
   object/sprinter.h
   object/sreader.h
   object/swriter.h
   object/syntax.h
   object/syntobject.h
   object/syntobject_d.h
   pattern/observable.h
   pattern/observer.h
   plugin/plugin.h
   sigc++/cartosigcmacro.h
   sigc++/cartosigcmacroend.h
   sigc++/sigc++.h
   sigc++/signal.h
   sigc++/trackable.h
   smart/mutexrcptr.h
   smart/rcobject.h
   smart/rcptr.h
   smart/rcptrtrick.h
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
   type/limits.h
   type/limits_gcc3.h
   type/select.h
   type/string_conversion.h
   type/types.h
   type/typetraits.h
   uuid/uuid.h
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
   zoolib/atomiccount.h
   zoolib/ZAtomic.h
   zoolib/zconfig.h
   zoolib/zconfigd.h
   zoolib/zconfigtest.h
   zoolib/ZDebug.h
)

BRAINVISA_COPY_AND_INSTALL_HEADERS( _headers cartobase headersTarget )

set( _cartobase_0_sources
  algorithm/algorithm.cc 
  allocator/allocator.cc 
  allocator/mappingcopy.cc 
  allocator/mappingro.cc 
  allocator/mappingrw.cc 
  allocator/meminfo.cc 
  config/paths.cc 
  config/verbose.cc 
  config/version.cc 
  datasource/bufferdatasource.cc 
  datasource/chaindatasource.cc 
  datasource/cstreamdatasource.cc 
  datasource/datasource.cc 
  datasource/fddatasource.cc 
  datasource/filedatasource.cc 
  datasource/filteringdatasource.cc 
  datasource/streamdatasource.cc 
  exception/assert.cc 
  exception/errno.cc 
  exception/file.cc 
  exception/format.cc 
  exception/ioexcept.cc 
  object/achecker.cc 
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
  type/string_conversion.cc 
  uuid/uuid.cc 
  io/asciidatasourcetraits.cc 
  io/formatdictionary.cc 
)

set( _cartobase_1_sources
  object/object_map.cc
  object/object_vector.cc
)

set( _cartobase_sources
  config/info.cc 
  getopt/getopt.cc 
  object/areader.cc 
  object/awriter.cc 
  object/object_factory.cc 
  object/object_map_string_vector.cc 
  object/object_rcptr_map.cc 
  object/object_rcptr_map_string_vector.cc 
  object/object_set.cc 
  object/pythonreader.cc 
  object/pythonwriter.cc 
  plugin/plugin.cc 
  io/datasourceinfo.cc 
  io/minfXML2.cc 
  io/pythonformatchecker.cc 
  io/pythonformatreader.cc 
  io/pythonformatwriter.cc 
  io/reader_object.cc 
  io/readeralgorithm.cc 
  io/writer.cc 
  io/xmlformatchecker.cc 
  io/xmlformatreader.cc
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
BRAINVISA_INSTALL( TARGETS cartobase_0
              DESTINATION lib
              COMPONENT ${PROJECT_NAME} )


add_library( cartobase_1 SHARED ${_cartobase_1_sources} )
set_property( TARGET cartobase_1 PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( cartobase_1 "cartobase_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
if(UNIX)
  target_link_libraries( cartobase_1 "pthread" )
endif(UNIX)
BRAINVISA_INSTALL( TARGETS cartobase_1
              DESTINATION lib
              COMPONENT ${PROJECT_NAME} )

              
add_library( cartobase SHARED ${_cartobase_sources} )
set_property( TARGET cartobase PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( cartobase "cartobase_1" "cartobase_0" ${LIBXML2_LIBRARIES} ${Sigc++2_LIBRARIES} )
if(UNIX)
  target_link_libraries( cartobase "pthread" )
endif(UNIX)
BRAINVISA_INSTALL( TARGETS cartobase
              DESTINATION lib
              COMPONENT ${PROJECT_NAME} )

              
IF ( DOXYGEN_FOUND )
  SET(component_name "cartobase")
  set( DOXYFILE_PREDEFINED "${AIMS_DEFINITIONS}")
  BRAINVISA_GENERATE_DOXYGEN_DOC( _headers 
                                  INPUT_PREFIX "${CMAKE_BINARY_DIR}/include/${component_name}"
                                  COMPONENT "${component_name}")
ENDIF ( DOXYGEN_FOUND )