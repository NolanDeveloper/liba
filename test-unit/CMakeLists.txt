cmake_minimum_required(VERSION 3.2)

project(liba_tests)

add_executable(liba_tests main.c)
target_include_directories(liba_tests PRIVATE ${acutest_SOURCE_DIR}/include)
target_link_libraries(liba_tests PUBLIC liba)
