## src/
AUTOMAKE_OPTIONS        = subdir-objects
AM_CPPFLAGS             = -std=c++11 \
	-I$(top_srcdir)/lib \
	${libconfig_CFLAGS} \
	${psocksxx_CFLAGS}

bitzincludedir          = $(pkgincludedir)
sbin_PROGRAMS           = bitz-server
bitz_server_LDFLAGS     = -ldl -lpthread -export-dynamic
bitz_server_LDADD       = $(top_builddir)/lib/icap/libicap.la \
	${libconfig_LIBS} \
	${psocksxx_LIBS}

bitz_server_SOURCES     = main.cpp \
	bitz-server.h bitz-server.cpp \
	bitz/exception.h bitz/exception.cpp \
	bitz/manager_exception.h bitz/manager_exception.cpp \
	bitz/config.h bitz/config.cpp \
	bitz/common.h \
	bitz/util.h bitz/util.cpp \
	bitz/manager.h bitz/manager.cpp \
	bitz/worker.h bitz/worker.cpp \
	bitz/request_handler.h bitz/request_handler.cpp \
	bitz/options_request_handler.h bitz/options_request_handler.cpp \
	bitz/reqmod_request_handler.h bitz/reqmod_request_handler.cpp \
	bitz/respmod_request_handler.h bitz/respmod_request_handler.cpp \
	bitz/modifier.cpp

bitzinclude_HEADERS     = \
	bitz/config.h \
	bitz/modifier.h

