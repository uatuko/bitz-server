## src/
AM_CPPFLAGS             = -I$(top_srcdir)/lib ${libconfig_CFLAGS} ${log4cpp_CFLAGS}

sbin_PROGRAMS           = bitz-server
bitz_server_LDADD       = $(top_builddir)/lib/socket/libsocket.la ${libconfig_LIBS} ${log4cpp_LIBS}
bitz_server_SOURCES     = bitz-server.cpp \
	bitz/exception.h bitz/exception.cpp \
	bitz/manager_exception.h bitz/manager_exception.cpp \
	bitz/config.h bitz/config.cpp \
	bitz/logger.h bitz/logger.cpp \
	bitz/manager.h bitz/manager.cpp \
	bitz/worket.h bitz/worker.cpp
