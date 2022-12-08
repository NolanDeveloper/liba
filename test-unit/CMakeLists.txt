add_executable(liba_tests main.c)
target_include_directories(liba_tests PRIVATE ${acutest_SOURCE_DIR}/include)
target_link_libraries(liba_tests PUBLIC liba)

add_test(NAME test-unit COMMAND liba_tests)