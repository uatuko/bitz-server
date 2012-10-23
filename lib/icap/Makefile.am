## [icap-library] lib/icap/
AM_CPPFLAGS             = -I$(top_srcdir)/lib

lib_LTLIBRARIES         = libicap.la
libicap_la_LIBADD       = $(top_builddir)/lib/socket/libsocket.la
libicap_la_SOURCES      = icap.h \
	header.h header.cpp \
	request_header.h request_header.cpp \
	util.h util.cpp

