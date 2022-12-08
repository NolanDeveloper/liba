add_library(liba liba.c compatibility.h)

target_link_libraries(liba PUBLIC liba_interface)
