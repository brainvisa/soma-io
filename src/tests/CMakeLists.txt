if( TIFF_FOUND )
  include_directories( ${TIFF_INCLUDE_DIR} )
endif()

BRAINVISA_ADD_EXECUTABLE(cartoAlgorithm_test       cartoAlgorithm_test/algorithm_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoConversion_test      cartoConversion_test/conversion_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoDataTypeTraits_test  cartoDataTypeTraits_test/datatypetraits_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoMinf_test            cartoMinf_test/cartoMinf_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoMultiThread_test     cartoMultiThread_test/main.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoMutex_test           cartoMutex_test/mutextest.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoPythonIO_test        cartoPythonIO_test/python_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoRcptr_test           cartoRcptr_test/rcptr_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoScopedptr_test       cartoScopedptr_test/scopedptr_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoSharedptr_test       cartoSharedptr_test/sharedptr_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoThreadedRcptr_test   cartoThreadedRcptr_test/threadedrcptr.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoUuid_test            cartoUuid_test/uuid_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoWeakptr_test         cartoWeakptr_test/weakptr_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoObject_test          cartoObject_test/object_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(cartoFileUtil_test        cartoFileUtil_test/fileutil_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(somaCapabilities_test     somaCapabilities_test/capabilities_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(somaDSList_test           somaDSList_test/dslist_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(somaRGB_test              somaRGB_test/rgb_test.cc HELP_GENERATE FALSE)
BRAINVISA_ADD_EXECUTABLE(somaGetPluginFile_test    somaGetPluginFile_test/getpluginfile_test.cc HELP_GENERATE FALSE)

#   BRAINVISA_ADD_EXECUTABLE(somaDSILoader_test      somaDSILoader_test/dsiloader_test.cc HELP_GENERATE FALSE)
#   BRAINVISA_ADD_EXECUTABLE(somaGisIR_test          somaGisIR_test/gisir_test.cc HELP_GENERATE FALSE)
#   BRAINVISA_ADD_EXECUTABLE(somaPartialIO_test      somaPartialIO_test/partialio_test.cc HELP_GENERATE FALSE)
#   BRAINVISA_ADD_EXECUTABLE(somaGisIW_test          somaGisIW_test/gisiw_test.cc HELP_GENERATE FALSE)


TARGET_LINK_LIBRARIES(cartoAlgorithm_test       cartobase)
TARGET_LINK_LIBRARIES(cartoConversion_test      cartobase)
TARGET_LINK_LIBRARIES(cartoDataTypeTraits_test  cartobase)
TARGET_LINK_LIBRARIES(cartoMultiThread_test     cartobase)
TARGET_LINK_LIBRARIES(cartoMutex_test           cartobase)
TARGET_LINK_LIBRARIES(cartoRcptr_test           cartobase)
TARGET_LINK_LIBRARIES(cartoScopedptr_test       cartobase)
TARGET_LINK_LIBRARIES(cartoSharedptr_test       cartobase)
TARGET_LINK_LIBRARIES(cartoThreadedRcptr_test   cartobase)
TARGET_LINK_LIBRARIES(cartoUuid_test            cartobase)
TARGET_LINK_LIBRARIES(cartoWeakptr_test         cartobase)
TARGET_LINK_LIBRARIES(cartoObject_test          cartobase)
TARGET_LINK_LIBRARIES(cartoPythonIO_test        soma-io)
TARGET_LINK_LIBRARIES(cartoMinf_test            soma-io)
TARGET_LINK_LIBRARIES(cartoFileUtil_test        soma-io)
TARGET_LINK_LIBRARIES(somaCapabilities_test     soma-io)
TARGET_LINK_LIBRARIES(somaDSList_test           soma-io)
TARGET_LINK_LIBRARIES(somaRGB_test              soma-io)
TARGET_LINK_LIBRARIES(somaGetPluginFile_test    soma-io)

install( TARGETS cartoRcptr_test cartoScopedptr_test
              cartoSharedptr_test cartoWeakptr_test cartoAlgorithm_test
              cartoConversion_test cartoDataTypeTraits_test
              cartoMultiThread_test cartoMutex_test cartoThreadedRcptr_test
              cartoUuid_test cartoObject_test somaCapabilities_test
              somaDSList_test
              DESTINATION bin
              COMPONENT ${PROJECT_NAME}-test )

# Plugins test
BRAINVISA_ADD_EXECUTABLE(somaGisFormatCheck_test   somaGisFormatCheck_test/gischeck_test.cc HELP_GENERATE FALSE)
TARGET_LINK_LIBRARIES(somaGisFormatCheck_test   soma-io cartobase)
if( TIFF_FOUND )
  BRAINVISA_ADD_EXECUTABLE(somaTiffFormatCheck_test  somaTiffFormatCheck_test/tiffcheck_test.cc HELP_GENERATE FALSE)
  BRAINVISA_ADD_EXECUTABLE(somaTiffIR_test           somaTiffIR_test/tiffir_test.cc HELP_GENERATE FALSE)
  TARGET_LINK_LIBRARIES(somaTiffFormatCheck_test  soma-io cartobase somatiff)
  TARGET_LINK_LIBRARIES(somaTiffIR_test           soma-io cartobase somatiff)
endif()


#   TARGET_LINK_LIBRARIES(somaDSILoader_test      ${SOMA-IO_LIBRARIES})
#   TARGET_LINK_LIBRARIES(somaGisIR_test          ${SOMA-IO_LIBRARIES})
#   TARGET_LINK_LIBRARIES(somaPartialIO_test      ${SOMA-IO_LIBRARIES})
#   TARGET_LINK_LIBRARIES(somaGisIW_test          ${SOMA-IO_LIBRARIES})

# -- tests --

brainvisa_add_test(cartoRcptr_test cartoRcptr_test)
brainvisa_add_test(cartoScopedptr_test cartoScopedptr_test)
brainvisa_add_test(cartoSharedptr_test cartoSharedptr_test)
brainvisa_add_test(cartoWeakptr_test cartoWeakptr_test)
brainvisa_add_test(cartoAlgorithm_test cartoAlgorithm_test)
brainvisa_add_test(cartoConversion_test cartoConversion_test)
brainvisa_add_test(cartoDataTypeTraits_test cartoDataTypeTraits_test)
brainvisa_add_test(cartoMultiThread_test cartoMultiThread_test)
brainvisa_add_test(cartoMutex_test cartoMutex_test)
brainvisa_add_test(cartoThreadedRcptr_test cartoThreadedRcptr_test)
brainvisa_add_test(cartoUuid_test cartoUuid_test)
brainvisa_add_test(cartoObject_test cartoObject_test)
brainvisa_add_test(somaCapabilities_test somaCapabilities_test)
brainvisa_add_test(somaDSList_test somaDSList_test)

