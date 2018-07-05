
add_library(mod_echo MODULE
	echo.cpp
)

set_target_properties(mod_echo PROPERTIES PREFIX "")

target_include_directories(mod_echo PRIVATE
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/src
)


install(
	TARGETS mod_echo
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/modules
)

