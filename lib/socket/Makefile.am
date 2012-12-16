## [socket-library] lib/
noinst_LTLIBRARIES      = libsocket.la
libsocket_la_SOURCES    = socket.h socket.cpp \
	securesocket.h securesocket.cpp

AM_CPPFLAGS             = @openssl_CFLAGS@
libsocket_la_LIBADD     = @openssl_LIBS@
