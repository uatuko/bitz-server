cmake_minimum_required (VERSION 3.5)
project (bitz-server VERSION 2.0.0 LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/dependencies.cmake)

add_subdirectory(conf)
add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(modules)
add_subdirectory(src)

