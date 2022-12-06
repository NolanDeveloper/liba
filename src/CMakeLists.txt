cmake_minimum_required(VERSION 3.2)

project(liba)

add_library(liba liba.c compatibility.h)
target_include_directories(liba PUBLIC ${LIBA_INCLUDE_DIRECTORY})
