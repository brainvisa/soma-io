PROJECT(SOMAIO)

cmake_minimum_required(VERSION 2.4)

IF(COMMAND cmake_policy)
  cmake_policy(VERSION 2.4)
  cmake_policy(SET CMP0002 NEW)
  cmake_policy(SET CMP0003 NEW)
#  cmake_policy(SET CMP0005 OLD)
ENDIF(COMMAND cmake_policy)

ADD_SUBDIRECTORY(src)

