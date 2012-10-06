## lib/
noinst_LTLIBRARIES 		= libsocket.la
libsocket_la_SOURCES	= socket.cpp \
	securesocket.cpp

INCLUDES				= -I$(top_srcdir)/include @openssl_CFLAGS@
libsocket_la_LIBADD		= @openssl_LIBS@
