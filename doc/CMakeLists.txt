
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


if(DOXYGEN_FOUND)
	configure_file(doxygen.cfg.in doxygen.cfg @ONLY)
	add_custom_target(doxygen
		COMMAND Doxygen::doxygen doxygen.cfg
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating doxygen docs"
		VERBATIM
	)
endif()

