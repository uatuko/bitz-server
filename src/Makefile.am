## src/
INCLUDES       = -I$(top_srcdir)/include

sbin_PROGRAMS  = server
server_SOURCES = server.cpp
server_LDADD   = $(top_builddir)/lib/libsocket.la

sbin_PROGRAMS            = server-preforked
server_preforked_SOURCES = server-preforked.cpp
server_preforked_LDADD   = $(top_builddir)/lib/libsocket.la

