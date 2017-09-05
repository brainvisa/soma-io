cmake_minimum_required( VERSION 2.6 )
find_package( brainvisa-cmake REQUIRED )
BRAINVISA_PROJECT()
enable_testing()

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

# PNG12 support library
find_package( PNG12 )
if( PNG12_FOUND )
  BRAINVISA_DEPENDENCY( RUN DEPENDS libpng12 RUN )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS libpng12 DEV )
endif()

# DICOM support library
find_package( DCMTK )
find_package( Jasper )
if( DCMTK_FOUND )
  if (DCMTK_oflog_INCLUDE_DIR AND JASPER_FOUND)
    # JPEG 2000 support is only available
    # for DICOM version >= 3.6, i.e. when
    # DCMTK_oflog_INCLUDE_DIR exists.
    set(SOMA_DICOM_JPEG2000_SUPPORT 1)
  endif()

  if (NOT DCMTK_oflog_INCLUDE_DIR)
     # For DICOM version < 3.6, dcmimage.so depends on PNG and TIFF libraries
     # but does not refer these libraries (especially on Ubuntu 12 and 14).
     # A reusable DCMTK_LIBRARIES_ADDITIONAL variable is defined to store these
     # dependencies. If these additional libraries are not used by programs that
     # link with dcmimage.so, linking may results in undefined symbols.
     set( DCMTK_LIBRARIES_ADDITIONAL
          ${PNG12_LIBRARIES}
          ${TIFF_LIBRARIES}
          CACHE INTERNAL "Additional libraries needed to link with DCMTK" )
  endif()

  BRAINVISA_DEPENDENCY( RUN DEPENDS dcmtk RUN )
  BRAINVISA_DEPENDENCY( DEV RECOMMENDS dcmtk DEV )

  if(SOMA_DICOM_JPEG2000_SUPPORT)
    BRAINVISA_DEPENDENCY( RUN DEPENDS jasper RUN )
    BRAINVISA_DEPENDENCY( DEV RECOMMENDS jasper DEV )
  endif()

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
BRAINVISA_DEPENDENCY( RUN RECOMMENDS python RUN ">= 2.6" )
BRAINVISA_DEPENDENCY( TST DEPENDS "${BRAINVISA_PACKAGE_NAME}" RUN "= ${${BRAINVISA_PACKAGE_NAME}_VERSION}" )

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

if (DCMTK_FOUND)
    set(SOMA-IO_INCLUDE_DIRS ${SOMA-IO_INCLUDE_DIRS}
        ${DCMTK_INCLUDE_DIR})
endif()

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
if( DCMTK_FOUND )
  add_subdirectory( src/somadicom )
endif()
add_subdirectory( src/tests )
find_package( Sphinx )

BRAINVISA_GENERATE_SPHINX_DOC( "sphinx"
  "share/doc/soma-io-${BRAINVISA_PACKAGE_VERSION_MAJOR}.${BRAINVISA_PACKAGE_VERSION_MINOR}" )


BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
