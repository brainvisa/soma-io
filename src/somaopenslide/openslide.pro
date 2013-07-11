TEMPLATE	= lib
TARGET		= somaopenslide${BUILDMODEEXT}

#!include ../../config

INCBDIR = soma

HEADERS = \
    checker/osformatchecker.h \
    image/osimagereader.h \
    image/osimagereader_d.h \
    plugin/osplugin.h

SOURCES = \
    checker/osformatchecker.cc \
    image/osimagereader.cc \
    plugin/osplugin.cc

LIBS	= 
