TEMPLATE	= lib
TARGET		= somagis${BUILDMODEEXT}

#!include ../../config

INCBDIR = soma-io

HEADERS = \
    checker/gisformatchecker.h \
    image/gisimagereader.h \
    image/gisimagereader_d.h \
    image/gisimagewriter.h	\
    image/gisimagewriter_d.h \
    plugin/gisplugin.h

SOURCES = \
    checker/gisformatchecker.cc \
    image/gisimagereader.cc \
    image/gisimagewriter.cc \
    plugin/gisplugin.cc

LIBS	= 
