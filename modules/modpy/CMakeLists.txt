
if(python_FOUND)
	add_library(mod_py MODULE
		py.cpp
		interface.cpp
	)

	set_target_properties(mod_py PROPERTIES PREFIX "")

	target_include_directories(mod_py PRIVATE
		${python_INCLUDE_DIRS}
		${spdlog_INCLUDE_DIRS}
	)

	target_include_directories(mod_py PUBLIC
		${CMAKE_SOURCE_DIR}/src
	)

	target_link_libraries(mod_py PRIVATE ${python_LDFLAGS})
	target_link_libraries(mod_py PUBLIC bitz-server)


	install(
		TARGETS mod_py
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/modules
	)

	install(
		FILES modules/modpy.py
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/modules/modpy
	)
endif()

