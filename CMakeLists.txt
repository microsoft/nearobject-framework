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

# Don't add CTest targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

include(CTest)
include(FetchContent)

# Configure GSL dependency
FetchContent_Declare(GSL
  GIT_REPOSITORY "https://github.com/microsoft/GSL"
  GIT_TAG "v4.0.0"
  GIT_SHALLOW ON
)

FetchContent_MakeAvailable(GSL)

set(JSON_ImplicitConversions OFF)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(magic_enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v0.8.1.tar.gz)
FetchContent_MakeAvailable(magic_enum)

find_package(Threads REQUIRED)

# Uncomment for verbose build information
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Use folders. In generated projects that support project organization, this
# allows specifying the 'FOLDER' property on a target, which will create a
# corresponding folder in the project. This is useful for MSBuild generated
# projects, where these folders will appear in IDEs that interpret them (eg.
# Visual Studio).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Common source directories
set(NOF_DIR_LIB ${CMAKE_CURRENT_LIST_DIR}/lib)
set(NOF_DIR_LINUX ${CMAKE_CURRENT_LIST_DIR}/linux)
set(NOF_DIR_WINDOWS ${CMAKE_CURRENT_LIST_DIR}/windows)

# Conditional inclusion of OS-dependent source trees.
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(BUILD_FOR_LINUX TRUE)
  add_subdirectory(linux)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Make public version of wil available.
  set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Turn off wil tests")
  set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Turn off wil packaging")

  FetchContent_Declare(WIL
      GIT_REPOSITORY "https://github.com/microsoft/wil"
      GIT_TAG "5eb59d60e167482639cc47ffb40442158da7fd04"
      GIT_SHALLOW OFF
  )
  FetchContent_MakeAvailable(WIL)

  # Add Windows-specific global compile definitions.
  add_compile_definitions(
      UNICODE
      NOMINMAX
      WIN32_LEAN_AND_MEAN
  )

  set(BUILD_FOR_WINDOWS TRUE)
  add_subdirectory(windows)
endif()

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)
