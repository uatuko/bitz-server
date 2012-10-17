## src/
AM_CPPFLAGS             = -I$(top_srcdir)/lib

sbin_PROGRAMS           = bitz-server
bitz_server_LDADD       = $(top_builddir)/lib/socket/libsocket.la
bitz_server_SOURCES     = bitz-server.cpp \
	bitz/manager.cpp
	bitz/worker.cpp
