cmake_minimum_required(VERSION 3.10)
project(lab3 C)

set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall)
add_compile_definitions(_POSIX_C_SOURCE=200809L)

find_package(Threads REQUIRED)

add_library(detlib STATIC src/matrix.c src/det.c)
target_include_directories(detlib PUBLIC src)
target_link_libraries(detlib PUBLIC Threads::Threads)

add_executable(det src/main.c)
target_link_libraries(det detlib)

