find_package( aims-free REQUIRED )
include( "${AIMS-FREE_USE_FILE}" )

add_executable( process_image_with_memory_mapping "process_image_with_memory_mapping.cpp" )
target_link_libraries( process_image_with_memory_mapping somaio ${AIMS-FREE_LIBRARIES})


add_executable( write_an_image "write_an_image.cpp" )
target_link_libraries( write_an_image somaio )
