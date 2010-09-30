TEMPLATE = lib
TARGET = $(TARGETLIB)

#!include ../../config-local
INCBDIR=

darwin:LIBS += -framework CoreServices

HEADERS = \
  soma/rcobject.hpp \
  soma/wip/image_reader.hpp \
  soma/wip/gis_format_reader.hpp \
  soma/wip/file_formats.hpp \



SOURCES = \
  rcobject.cpp \
  soma/wip/gis_format_reader.cpp \


