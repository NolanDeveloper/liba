cmake_minimum_required(VERSION 3.2)

project(liba)

set(LIBA_IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(LIBA_IS_MAIN_PROJECT ON)
endif()

include(FetchContent)

set(CMAKE_C_STANDARD 11)
set(LIBA_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

option(LIBA_BUILD_TESTS "Build test programs" ${LIBA_IS_MAIN_PROJECT})
if(LIBA_BUILD_TESTS)
    FetchContent_Declare(
            acutest
            GIT_REPOSITORY https://github.com/mity/acutest
            GIT_TAG        cce300734bfe5c3879b0449ac283a872633e615c
    )
    FetchContent_MakeAvailable(acutest)

    enable_testing()
    add_subdirectory(test-unit)
endif ()

