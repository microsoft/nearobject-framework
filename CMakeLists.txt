cmake_minimum_required(VERSION 3.11)

project(nearobject-framework LANGUAGES CXX)

# Set language configutation options. The C++ standard used must be the lowest
# common denominator for all the OS-dependent projects. In practice, since this
# project must build under the Windows build system (build.exe), its toolchain
# is typically the limiting factor.
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

# Use folders. In generated projects that support project organization, this
# allows specifying the 'FOLDER' property on a target, which will create a
# corresponding folder in the project. This is useful for MSBuild generated
# projects, where these folders will appear in IDEs that interpret them (eg.
# Visual Studio).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Common source directories
set(UWB_DIR_LIB ${CMAKE_CURRENT_LIST_DIR}/lib)
set(UWB_DIR_LINUX ${CMAKE_CURRENT_LIST_DIR}/linux)
set(UWB_DIR_WINDOWS ${CMAKE_CURRENT_LIST_DIR}/windows)

add_subdirectory(lib)
add_subdirectory(tools)

# Conditional inclusion of OS-dependent source trees.
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_subdirectory(linux)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  add_subdirectory(windows)
endif()
