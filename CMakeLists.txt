cmake_minimum_required(VERSION 3.10)

project(
    heist
    VERSION 1.0
    DESCRIPTION "Just a tity C library."
)

add_executable(heist 
    src/main.c 
    src/harray.c
    src/hlist.c 
    src/hmap.c
)

target_include_directories(heist PUBLIC include)
