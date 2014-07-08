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

find_package( Doxygen )

# OpenSlide support library
find_package( OpenSlide )
if( OPENSLIDE_FOUND )
  BRAINVISA_DEPENDENCY( RUN DEPENDS libopenslide RUN )
  BRAINVISA_VERSION_CONVERT( OPENSLIDE_HEX_VERSION ${OPENSLIDE_VERSION} HEX )
endif()
BRAINVISA_DEPENDENCY( DEV RECOMMENDS libopenslide DEV )

# TIFF support library
find_package( TIFF )
if( TIFF_FOUND )
  BRAINVISA_DEPENDENCY( RUN DEPENDS libtiff RUN )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS libtiff DEV )
endif()

# DICOM support library
find_package( DCMTK )
find_package( Jasper )
# find_package( TIFF ) # already done
find_package( PNG12 )
if( DCMTK_FOUND AND JASPER_FOUND AND TIFF_FOUND AND PNG12_FOUND )
  BRAINVISA_DEPENDENCY( RUN DEPENDS dcmtk RUN )
  BRAINVISA_DEPENDENCY( RUN DEPENDS jasper RUN )
#  BRAINVISA_DEPENDENCY( RUN DEPENDS tiff RUN )
  BRAINVISA_DEPENDENCY( RUN DEPENDS libpng12 RUN )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS dcmtk DEV )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS jasper DEV )
#  BRAINVISA_DEPENDENCY( DEV RECOMMENDS tiff DEV )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS libpng12 DEV )
endif()

find_package( ZLIB )
if( ZLIB_FOUND )
  BRAINVISA_DEPENDENCY( RUN DEPENDS zlib RUN )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS zlib DEV )
endif()

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

## Flags definitions
set( SOMA-IO_DEFINITIONS "-D_REENTRANT" )
if( "${CARTO_DEBUGMODE}" STREQUAL "debug" )
  set( CARTO_DEBUG 1 )
  set( SOMA-IO_DEFINITIONS ${SOMA-IO_DEFINITIONS} "-DCARTO_DEBUG" )
else()
  set( CARTO_DEBUG 0 )
endif()
add_definitions(${SOMA-IO_DEFINITIONS})

## Include directories
set( SOMA-IO_INCLUDE_DIRS 
     ${Sigc++2_INCLUDE_DIRS} 
     "${LIBXML2_INCLUDE_DIR}"
     ${Boost_INCLUDE_DIRS}
   )

include_directories( ${SOMA-IO_INCLUDE_DIRS} )

## Libraries

## Subdirectories
add_subdirectory( src/libcartobase )
add_subdirectory( src/libsoma-io )
add_subdirectory( src/somagis )
if( ZLIB_FOUND )
  add_subdirectory( src/somanifti )
endif()
if( OPENSLIDE_FOUND )
  add_subdirectory( src/somaopenslide )
endif()
if( TIFF_FOUND )
  add_subdirectory( src/somatiff )
endif()
if( DCMTK_FOUND AND JASPER_FOUND AND TIFF_FOUND AND PNG12_FOUND )
  add_subdirectory( src/somadicom )
endif()
add_subdirectory( src/tests )

BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
