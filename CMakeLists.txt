cmake_minimum_required( VERSION 3.29 )
enable_testing()

project("soma-io")
set(soma-io_VERSION 5.2.0)
set(soma-io_VERSION_MAJOR 5)
set(soma-io_VERSION_MINOR 2)
set(soma-io_VERSION_PATCH 0)

function( BRAINVISA_GENERATE_TARGET_NAME _variableName )
  if( DEFINED ${PROJECT_NAME}_TARGET_COUNT )
    math( EXPR ${PROJECT_NAME}_TARGET_COUNT ${${PROJECT_NAME}_TARGET_COUNT}+1 )
    set( ${PROJECT_NAME}_TARGET_COUNT ${${PROJECT_NAME}_TARGET_COUNT} CACHE INTERNAL "Used to generate new targets" )
  else( DEFINED ${PROJECT_NAME}_TARGET_COUNT )
    set( ${PROJECT_NAME}_TARGET_COUNT 1 CACHE INTERNAL "Used to generate new targets" )
  endif( DEFINED ${PROJECT_NAME}_TARGET_COUNT )
  set( ${_variableName} ${PROJECT_NAME}_target_${${PROJECT_NAME}_TARGET_COUNT} PARENT_SCOPE )
endfunction( BRAINVISA_GENERATE_TARGET_NAME )


function( BRAINVISA_COPY_AND_INSTALL_HEADERS _headersVariable _includeDir targetVariable )
  if( "${ARGV3}" STREQUAL "NO_SYMLINKS" )
    set( symlinks FALSE )
  else()
    set( symlinks TRUE )
  endif()

  set( destHeaders )
  foreach( _currentHeader ${${_headersVariable}} )
    set( _destFile "${CMAKE_BINARY_DIR}/include/${_includeDir}/${_currentHeader}" )
    if( symlinks AND ( UNIX OR APPLE OR CMAKE_CROSSCOMPILING) )
      # Make a symlink instead of copying Python source allows to
      # execute code from the build tree and directly benefit from
      # modifications in the source tree (without typing make)
      get_filename_component( _path "${_destFile}" PATH )
      file( RELATIVE_PATH _relsource ${_path}
            "${CMAKE_CURRENT_SOURCE_DIR}/${_currentHeader}" )
      add_custom_command(
        OUTPUT "${_destFile}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_currentHeader}"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${_path}"
        COMMAND "${CMAKE_COMMAND}" -E create_symlink "${_relsource}" "${_destFile}" )
    else()
      add_custom_command(
        OUTPUT "${_destFile}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_currentHeader}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${_currentHeader}" "${_destFile}" )
    endif()
    set( destHeaders ${destHeaders}  "${_destFile}" )
    get_filename_component( _path "${_currentHeader}" PATH )
    install( FILES ${_currentHeader}
                       DESTINATION include/${_includeDir}/${_path}
                       COMPONENT ${PROJECT_NAME}-dev )
  endforeach()
  BRAINVISA_GENERATE_TARGET_NAME( target )
  add_custom_target( ${target} ALL
                     DEPENDS ${destHeaders} )
  set( ${targetVariable} "${target}" PARENT_SCOPE )
endfunction()


function( BRAINVISA_GENERATE_DOXYGEN_DOC )
endfunction()

function( BRAINVISA_ADD_EXECUTABLE name src )
  add_executable(${name} "${src}")
endfunction()

function( BRAINVISA_ADD_TEST )
endfunction()

include_directories("${CMAKE_BINARY_DIR}/include")

## Lib dependencies
include(FindPkgConfig)
pkg_check_modules(DEPS REQUIRED sigc++-2.0)
include_directories(${DEPS_INCLUDE_DIRS})
#target_link_libraries(yourprogram ${DEPS_LIBRARIES})


find_package( LibXml2 REQUIRED )

find_package( Boost REQUIRED )

find_package( Doxygen )

# OpenSlide support library
find_package( OpenSlide )

# DICOM support library
find_package( DCMTK )
find_package( Jasper QUIET )
find_package( OpenJpeg QUIET )
if( DCMTK_FOUND )
  if (DCMTK_oflog_INCLUDE_DIR AND ( JASPER_FOUND OR OPENJPEG_FOUND ) )
    # JPEG 2000 support is only available
    # for DICOM version >= 3.6, i.e. when
    # DCMTK_oflog_INCLUDE_DIR exists.
    set(SOMA_DICOM_JPEG2000_SUPPORT 1)
  endif()

  if(SOMA_DICOM_JPEG2000_SUPPORT)
    if( JASPER_FOUND )
      add_definitions( -DHAS_JASPER )
    else( JASPER_FOUND )
      if( OPENJPEG_FOUND )
        string( REPLACE "." "" OPJ_VERSION_STR ${OPENJPEG_VERSION} )
        add_definitions( -DHAS_OPENJPEG -DOPENJPEG_VERSION=${OPJ_VERSION_STR})
      endif( OPENJPEG_FOUND )
    endif( JASPER_FOUND )
  endif()

endif()

find_package( ZLIB )

# # Python is required for the DICOM conversion script
# # It should be made optional
# find_package( python REQUIRED )


message( "WARNING: project brainvisa-share has not been found. Some programs that require access to shared directories may fail to run." )

# BRAINVISA_COPY_PYTHON_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/python"
#                                  ${PROJECT_NAME} )
# BRAINVISA_COPY_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/bin"
#                           bin
#                           ${PROJECT_NAME} )
# BRAINVISA_COPY_DIRECTORY( "${CMAKE_CURRENT_SOURCE_DIR}/share" 
#                           "share/${PROJECT_NAME}-${BRAINVISA_PACKAGE_VERSION_MAJOR}.${BRAINVISA_PACKAGE_VERSION_MINOR}"
#                           ${PROJECT_NAME} )

if(NOT CARTO_DEBUGMODE)
  if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" 
     OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" 
     OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel" )
        set( CARTO_DEBUGMODE "release" )
  else()
    if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
        set( CARTO_DEBUGMODE "debug" )
    else()
      set( CARTO_DEBUGMODE "default" )
    endif()
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

if(CARTO_DEBUGREF)
  set( SOMA-IO_DEFINITIONS ${SOMA-IO_DEFINITIONS} "-DDEBUG_REF" )
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
  add_subdirectory( src/somamif )
  add_subdirectory( src/somaminc )
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


# BRAINVISA_GENERATE_SPHINX_DOC( "sphinx"
#   "share/doc/soma-io-${BRAINVISA_PACKAGE_VERSION_MAJOR}.${BRAINVISA_PACKAGE_VERSION_MINOR}" )


# BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
