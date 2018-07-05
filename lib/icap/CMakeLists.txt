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

set(ICAP_VERSION_CURRENT  1)
set(ICAP_VERSION_REVISION 0)
set(ICAP_VERSION_AGE      0)
set(ICAP_VERSION  "${ICAP_VERSION_CURRENT}.${ICAP_VERSION_REVISION}.${ICAP_VERSION_AGE}")
set(ICAP_SOVERSION ${ICAP_VERSION_CURRENT})

add_library(icap
	header.cpp
	request_header.cpp
	response_header.cpp
	request.cpp
	response.cpp
	util.cpp
)

target_include_directories(icap PRIVATE
	${CMAKE_BINARY_DIR}/include
	${psocksxx_INCLUDE_DIRS}
)

set_target_properties(
	icap
	PROPERTIES
		VERSION   ${ICAP_VERSION}
		SOVERSION ${ICAP_SOVERSION}
)

include(GNUInstallDirs)
install(
	TARGETS icap
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	FILES
		common.h
		header.h
		request_header.h response_header.h
		request.h response.h
		util.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/icap
)

