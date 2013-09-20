## [icap-library] lib/icap/
AM_CPPFLAGS             = -I$(top_srcdir)/lib ${psocksxx_CFLAGS}
libicapincludedir       = $(includedir)/icap

lib_LTLIBRARIES         = libicap.la
libicap_la_LIBADD       = ${psocksxx_LIBS}
libicap_la_LDFLAGS      = -version-info @ICAP_LT_VERSION@ -no-undefined
libicap_la_SOURCES      = \
	header.cpp \
	request_header.cpp \
	response_header.cpp \
	request.cpp \
	response.cpp \
	util.cpp

libicapinclude_HEADERS  = \
	common.h \
	header.h  \
	request_header.h \
	response_header.h \
	request.h \
	response.h \
	util.h

