include(GNUInstallDirs)

# FIXME: These are temporary, should update *.in files to use cmake variables
set(sysconfdir ${CMAKE_INSTALL_FULL_SYSCONFDIR})
set(localstatedir ${CMAKE_INSTALL_FULL_LOCALSTATEDIR})
set(pkglibdir ${CMAKE_INSTALL_FULL_LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(bitz-server.conf.in bitz-server.conf @ONLY)

install(
	FILES ${CMAKE_BINARY_DIR}/conf/bitz-server.conf
	DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/bitz
)

