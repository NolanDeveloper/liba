cmake_minimum_required(VERSION 3.2)

project(liba_tests)

add_executable(liba_tests main.c)
target_include_directories(liba_tests PRIVATE "${CMAKE_SOURCE_DIR}/submodules/acutest/include")
target_link_libraries(liba_tests PUBLIC liba)
