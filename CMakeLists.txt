cmake_minimum_required(VERSION 3.21)

project(winuwb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Uncomment for verbose build information
# set(CMAKE_VERBOSE_MAKEFILE ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Common source directories
set(UWB_DIR_LIB ${CMAKE_CURRENT_LIST_DIR}/lib)
set(UWB_DIR_WINDOWS ${CMAKE_CURRENT_LIST_DIR}/windows)

add_subdirectory(lib)
add_subdirectory(linux)
add_subdirectory(windows)
