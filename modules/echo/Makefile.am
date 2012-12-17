# modules/echo
AM_CPPFLAGS             = -I$(top_srcdir)/lib -I$(top_srcdir)/src

modlibdir               = $(sysconfdir)/bitz/modules
modlib_LTLIBRARIES      = mod_echo.la
mod_echo_la_LDFLAGS     = -module -avoid-version -no-static
mod_echo_la_SOURCES     = \
	echo.h echo.cpp

