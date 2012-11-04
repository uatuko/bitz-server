## [icap-library] lib/icap/
AM_CPPFLAGS             = -I$(top_srcdir)/lib

lib_LTLIBRARIES         = libicap.la
libicap_la_LIBADD       = $(top_builddir)/lib/socket/libsocket.la
libicap_la_LDFLAGS      = -version-info @ICAP_LT_VERSION@ -no-undefined
libicap_la_SOURCES      = icap.h \
	header.h header.cpp \
	request_header.h request_header.cpp \
	response_header.h response_header.cpp \
	response.h response.cpp \
	util.h util.cpp

