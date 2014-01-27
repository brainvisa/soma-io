TEMPLATE	= lib
TARGET		= somadicom${BUILDMODEEXT}

#!include ../../config

INCBDIR = soma-io

HEADERS = \
    checker/dicomformatchecker.h \
    image/dicomimagereader.h \
    image/dicomimagereader_d.h \
    plugin/dicomplugin.h

SOURCES = \
    checker/dicomformatchecker.cc \
    image/dicomimagereader.cc \
    plugin/dicomplugin.cc

LIBS	= 
