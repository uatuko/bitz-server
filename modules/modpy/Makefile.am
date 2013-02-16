# modules/modpy

if MODPY
modlibdir               = $(sysconfdir)/bitz/modules
modlib_LTLIBRARIES      = mod_py.la
mod_py_la_LDFLAGS       = -module -avoid-version
mod_py_la_SOURCES       = \
	py.h py.cpp \
	interface.h interface.cpp

AM_CPPFLAGS             = -I$(top_srcdir)/lib -I$(top_srcdir)/src \
	${PYTHON_INCLUDES} ${libconfig_CFLAGS}
	
mod_py_la_LIBADD        = ${PYTHON_LIBS}
endif

