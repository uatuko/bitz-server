
if(python_FOUND)
	add_library(mod_py MODULE
		py.cpp
		interface.cpp
	)

	set_target_properties(mod_py PROPERTIES PREFIX "")

	target_include_directories(mod_py PRIVATE
		${CMAKE_SOURCE_DIR}/lib
		${CMAKE_SOURCE_DIR}/src
		${python_INCLUDE_DIRS}
		${libconfig_INCLUDE_DIRS}
	)

	target_link_libraries(mod_py PRIVATE ${python_LDFLAGS})


	install(
		TARGETS mod_py
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/modules
	)

	install(
		FILES modules/modpy.py
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/modules/modpy
	)
endif()

