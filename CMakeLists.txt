cmake_minimum_required(VERSION 3.11)

project(slrp-framework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Configure GSL dependency
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)

# Uncomment for verbose build information
# set(CMAKE_VERBOSE_MAKEFILE ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Common source directories
set(UWB_DIR_LIB ${CMAKE_CURRENT_LIST_DIR}/lib)
set(UWB_DIR_WINDOWS ${CMAKE_CURRENT_LIST_DIR}/windows)
set(UWB_DIR_LINUX ${CMAKE_CURRENT_LIST_DIR}/linux)

add_subdirectory(lib)
add_subdirectory(linux)
add_subdirectory(windows)
