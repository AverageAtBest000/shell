cmake_minimum_required(VERSION 3.16)

project(shell LANGUAGES C)

add_executable(shell src/main.c src/builtins.c src/executor.c src/input.c src/parser.c)

target_include_directories(shell PRIVATE include)

target_compile_options(shell PRIVATE -Wall -Wextra)
