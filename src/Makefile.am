lib_headers = knot_protocol.h knot_types.h
lib_sources = knot_protocol.c

libknotprotocol_includedir = $(includedir)/knot
libknotprotocol_include_HEADERS = $(lib_headers)

lib_LTLIBRARIES = libknotprotocol.la
libknotprotocol_la_SOURCES = $(lib_headers) $(lib_sources)
libknotprotocol_la_LDFLAGS = $(AM_LDFLAGS)

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = knotprotocol.pc
