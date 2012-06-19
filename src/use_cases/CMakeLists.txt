brainvisa_add_executable( write_an_image "write_an_image.cpp" )
target_link_libraries( write_an_image somaio )

find_package( aims-free )
if ( aims-free_FOUND )
  include( "${AIMS-FREE_USE_FILE}" )

  brainvisa_add_executable( process_image_with_memory_mapping "process_image_with_memory_mapping.cpp" HELP_GENERATE FALSE)
  target_link_libraries( process_image_with_memory_mapping somaio ${AIMS-FREE_LIBRARIES})
endif()