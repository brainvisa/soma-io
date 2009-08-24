cmake_minimum_required( VERSION 2.6 )

find_package( brainvisa-cmake REQUIRED )

BRAINVISA_PROJECT()

find_package( Doxygen )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

add_subdirectory( src )

BRAINVISA_CREATE_CMAKE_CONFIG_FILES()
