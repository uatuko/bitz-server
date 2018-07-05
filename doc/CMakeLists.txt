
include(GNUInstallDirs)
install(
	FILES bitz-server.man
	DESTINATION ${CMAKE_INSTALL_MANDIR}/man1
	RENAME bitz-server.1
)

install(
	FILES bitz-server.conf.man
	DESTINATION ${CMAKE_INSTALL_MANDIR}/man5
	RENAME bitz-server.conf.5
)

