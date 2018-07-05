
add_executable(bitz-server
	main.cpp
	bitz-server.cpp
	bitz/exception.cpp
	bitz/manager_exception.cpp
	bitz/config.cpp
	bitz/util.cpp
	bitz/manager.cpp
	bitz/worker.cpp
	bitz/request_handler.cpp
	bitz/options_request_handler.cpp
	bitz/reqmod_request_handler.cpp
	bitz/respmod_request_handler.cpp
	bitz/modifier.cpp
)

set_target_properties(bitz-server PROPERTIES LINK_FLAGS "-export-dynamic")

target_include_directories(bitz-server PRIVATE
	${CMAKE_BINARY_DIR}/include
	${CMAKE_SOURCE_DIR}/lib
	${psocksxx_INCLUDE_DIRS}
	${spdlog_INCLUDE_DIRS}
)

target_link_libraries(bitz-server
	dl pthread
	icap
	${psocksxx_LDFLAGS}
	${libconfig_LDFLAGS}
)


include(GNUInstallDirs)
install(
	TARGETS bitz-server
	RUNTIME DESTINATION ${CMAKE_INSTALL_SBINDIR}
)

install(
	FILES
		bitz/config.h
		bitz/modifier.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
)

