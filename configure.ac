# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([bitz-server], [0.0.1], [https://github.com/uditha-atukorala/bitz-server/issues])
AC_CONFIG_AUX_DIR([aux-build])
AC_CONFIG_MACRO_DIR([aux-build/m4])
AC_CONFIG_HEADERS([include/config.h])

# Versioning rules ( C:R:A )
#
# 1. Start with version 0:0:0.
# 2. If any of the sources have changed, increment R. This is a new revision
#    of the current interface.
# 3. If the interface has changed, increment C and set R to 0. This is the
#    first revision of a new interface.
# 4. If the new interface is a superset of the previous interface
#    (that is, if the previous interface has not been broken by the
#    changes in this new release), increment A. This release is backwards
#    compatible with the previous release.
# 5. If the new interface has removed elements with respect to the
#    previous interface, then backward compatibility is broken; set A to 0.
#    This release has a new, but backwards incompatible interface.
#
# For more info see section 6.3 of the GNU Libtool Manual.
#
# In short;
# +1 : ? : +1  == new interface that does not break old one
# +1 : ? : 0   == new interface that breaks old one
#  ? : ? : 0   == no new interfaces, but breaks apps
#  ? :+1 : ?   == just some internal changes, nothing breaks but might work
#                 better
# CURRENT : REVISION : AGE

LT_VERSION=0:0:1
AC_SUBST(LT_VERSION)

# Init
AM_INIT_AUTOMAKE
LT_INIT()

# Check for programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_MKDIR_P
AC_PROG_LIBTOOL

# Language
AC_LANG(C++)

# Checks for libraries
AM_PROG_LIBTOOL

AC_CHECK_LIB([pthread], [pthread_create],, AC_MSG_ERROR([usable pthread library now found]))

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([libconfig], [libconfig++ >= 1.4],,
	AC_MSG_ERROR([libconfig++ 1.4 or newer not found.])
)
PKG_CHECK_MODULES([log4cpp], [log4cpp >= 1.0],,
	AC_MSG_ERROR([log4cpp 1.0 or newer not found.])
)
PKG_CHECK_MODULES([openssl], [openssl >= 1.0.1],,
	AC_MSG_ERROR([openssl 1.0.1 or newer not found.])
)

# Checks for header files.
#AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/socket.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_CHECK_FUNCS([gethostbyname inet_ntoa memmove memset select socket strchr strerror])

AC_SUBST(openssl_LIBS)
AC_SUBST(openssl_CFLAGS)

pkgconfig_LIBS+="${openssl_LIBS}"
AC_SUBST(pkgconfig_LIBS)


AC_CONFIG_FILES([ \
	Makefile \
	conf/Makefile \
	include/Makefile \
	lib/Makefile \
	lib/socket/Makefile \
	src/Makefile \
])

AC_OUTPUT
