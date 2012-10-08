## src/
INCLUDES       = -I$(top_srcdir)/include

sbin_PROGRAMS  = server server-preforked
server_SOURCES = server.cpp
server_LDADD   = $(top_builddir)/lib/libsocket.la

server_preforked_SOURCES = server-preforked.cpp
server_preforked_LDADD   = $(top_builddir)/lib/libsocket.la

bin_PROGRAMS   = client
client_SOURCES = client.cpp
client_LDADD   = $(top_builddir)/lib/libsocket.la
