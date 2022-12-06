cmake_minimum_required(VERSION 3.2)

project(liba)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_C_STANDARD 11)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(LIBA_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test-unit)
add_subdirectory(submodules/acutest)
