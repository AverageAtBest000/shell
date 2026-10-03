cmake_minimum_required(VERSION 3.16)

project(shell LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(shell src/main.c src/builtins.c src/executor.c src/input/input.c src/parser.c)

include(FetchContent)

FetchContent_Declare(libds-c
    GIT_REPOSITORY https://github.com/Villy-P/libds-c.git
    GIT_TAG        main # consider pinning to a release tag or commit SHA for reproducible builds
)
FetchContent_MakeAvailable(libds-c)

target_link_libraries(shell PRIVATE libds-c::libds-c)

target_include_directories(shell PRIVATE include)

target_compile_options(shell PRIVATE -Wall -Wextra)
