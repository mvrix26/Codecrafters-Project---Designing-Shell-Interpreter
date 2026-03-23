cmake_minimum_required(VERSION 3.13)

project(codecrafters-shell)

file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(shell ${SOURCE_FILES}
        src/parser.c
        src/parser.h
        src/builtins.c
        src/builtins.h
        src/executor.c
        src/executor.h)

target_link_libraries(shell PRIVATE readline)
