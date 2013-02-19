## src/
AM_CPPFLAGS             = -I$(top_srcdir)/lib ${libconfig_CFLAGS} ${log4cpp_CFLAGS}

bitzincludedir          = $(includedir)/bitz
sbin_PROGRAMS           = bitz-server
bitz_server_LDFLAGS     = -export-dynamic
bitz_server_LDADD       = $(top_builddir)/lib/socket/libsocket.la \
	$(top_builddir)/lib/icap/libicap.la \
	${libconfig_LIBS} ${log4cpp_LIBS}

bitz_server_SOURCES     = bitz-server.cpp \
	bitz/exception.h bitz/exception.cpp \
	bitz/manager_exception.h bitz/manager_exception.cpp \
	bitz/config.h bitz/config.cpp \
	bitz/logger.h bitz/logger.cpp \
	bitz/common.h \
	bitz/util.h bitz/util.cpp \
	bitz/manager.h bitz/manager.cpp \
	bitz/worket.h bitz/worker.cpp \
	bitz/request_handler.h bitz/request_handler.cpp \
	bitz/options_request_handler.h bitz/options_request_handler.cpp \
	bitz/reqmod_request_handler.h bitz/reqmod_request_handler.cpp \
	bitz/modifier.cpp

bitzinclude_HEADERS     = \
	bitz/config.h \
	bitz/logger.h \
	bitz/modifier.h

