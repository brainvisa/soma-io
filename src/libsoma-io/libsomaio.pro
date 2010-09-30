TEMPLATE = lib
TARGET = $(TARGETLIB)

#!include ../../config-local
INCBDIR=

darwin:LIBS += -framework CoreServices

HEADERS = \
  soma/rcobject.hpp



SOURCES = \
  rcobject.cpp



