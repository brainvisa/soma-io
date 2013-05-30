cmake_minimum_required( VERSION 2.6 )
find_package( brainvisa-cmake REQUIRED )
BRAINVISA_PROJECT()

## Lib dependencies
find_package( Sigc++2 REQUIRED )
BRAINVISA_DEPENDENCY( RUN DEPENDS libsigc++2 RUN 2.0-0c2a )
BRAINVISA_DEPENDENCY( DEV DEPENDS libsigc++2 DEV 2.0 )

find_package( LibXml2 REQUIRED )
BRAINVISA_DEPENDENCY( RUN DEPENDS libxml2 RUN )
BRAINVISA_DEPENDENCY( DEV DEPENDS libxml2 DEV )

find_package( Boost REQUIRED )
BRAINVISA_DEPENDENCY( DEV DEPENDS libboost DEV )

# Python is required for the DICOM conversion script
# It should be made optional
find_package( python REQUIRED )

BRAINVISA_DEPENDENCY( DEV DEPENDS "${BRAINVISA_PACKAGE_NAME}" RUN "= ${${BRAINVISA_PACKAGE_NAME}_VERSION}" )
BRAINVISA_DEPENDENCY( RUN DEPENDS python RUN ">= ${PYTHON_SHORT_VERSION}" )
BRAINVISA_DEPENDENCY( DEV DEPENDS python DEV ">= 2.5;<< 3.0" )

# used for brainvisa-share/config.h
BRAINVISA_FIND_PACKAGE( brainvisa-share QUIET )
if( BRAINVISA-SHARE_FOUND )
  include( "${BRAINVISA-SHARE_USE_FILE}" )
else()
  message( "WARNING: project brainvisa-share has not been found. Some programs that require access to shared directories may fail to run." )
endif()

BRAINVISA_COPY_PYTHON_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/python"
                                 ${PROJECT_NAME} )
BRAINVISA_COPY_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/bin"
                          bin
                          ${PROJECT_NAME} )
BRAINVISA_COPY_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/share" 
                          "share/${PROJECT_NAME}-${BRAINVISA_PACKAGE_VERSION_MAJOR}.${BRAINVISA_PACKAGE_VERSION_MINOR}"
                          ${PROJECT_NAME} )

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel" )
  set( CARTO_DEBUGMODE "release" )
else()
  if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    set( CARTO_DEBUGMODE "debug" )
  else()
    set( CARTO_DEBUGMODE "default" )
  endif()
endif()

## Enabling of SOMA-IO
## /!\ It is then used by the whole aims project
set( USE_SOMA_IO "False" CACHE BOOL "Enable/Disable use of new soma-io classes")

## Flags definitions
set( SOMA-IO_DEFINITIONS "-D_REENTRANT" )
if( USE_SOMA_IO )
  set( SOMA-IO_DEFINITIONS ${SOMA-IO_DEFINITIONS} "-DUSE_SOMA_IO" )
  add_definitions(${SOMA-IO_DEFINITIONS})
endif()

## Include directories
set( SOMA-IO_INCLUDE_DIRS 
     ${Sigc++2_INCLUDE_DIRS} 
     "${LIBXML2_INCLUDE_DIR}"
     ${Boost_INCLUDE_DIRS}
   )
include_directories( ${SOMA-IO_INCLUDE_DIRS} )

## Libraries
set( SOMA-IO_LIBRARIES cartobase "${LIBXML2_LIBRARIES}" )
if( USE_SOMA_IO )
  set( SOMA-IO_LIBRARIES soma-io "${SOMA-IO_LIBRARIES}" )
endif()

## Subdirectories
add_subdirectory( src/libcartobase )
if( USE_SOMA_IO )
  add_subdirectory( src/libsoma-io )
endif()
add_subdirectory( src/tests )

BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
