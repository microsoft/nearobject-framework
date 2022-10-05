cmake_minimum_required(VERSION 3.12)

# Configuration option for using vcpkg to satisfy source package dependencies.
# This must be set to ON for official Windows builds.
option(USE_VCPKG "Use vcpkg for obtaining source dependencies" OFF)

# Conditional inclusion of OS-dependent source trees.
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(BUILD_FOR_LINUX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(BUILD_FOR_WINDOWS TRUE)
endif()

# Select source package dependency manager based on selected option.
# This must occur prior to the first project() statement.
if (USE_VCPKG)
  find_package(Git REQUIRED)
  set(VCPKG_SUBMODULE_ROOT ${CMAKE_CURRENT_LIST_DIR}/vcpkg)

  # Initialize vcpkg sub-module if not already done.
  if (NOT EXISTS ${VCPKG_SUBMODULE_ROOT}/.git)
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${VCPKG_SUBMODULE_ROOT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} 
      COMMAND_ERROR_IS_FATAL ANY)
  endif()

  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
  MESSAGE(STATUS "using vcpkg for source dependencies")
else()
  MESSAGE(STATUS "using FetchContent for source dependencies")
endif()

project(nearobject-framework LANGUAGES CXX)

# Set language configutation options. The C++ standard used must be the lowest
# common denominator for all the OS-dependent projects. In practice, since this
# project must build under the Windows build system (build.exe), its toolchain
# is typically the limiting factor.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Don't add CTest targets
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)

include(CTest)
include(FetchContent)

# Set external dependency target versions.
set(VERSION_NLOHMANN_JSON 3.11.2)
set(VERSION_MAGIC_ENUM 0.8.1)
set(VERSION_CLI11 2.2.0)
set(VERSION_CATCH2 3.1.0)

# Pull in external dependencies.
if (USE_VCPKG)
  set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
  find_package(nlohmann_json CONFIG ${VERSION_NLOHMANN_JSON} REQUIRED)
  find_package(magic_enum CONFIG ${VERSION_MAGIC_ENUM} REQUIRED)
  find_package(CLI11 CONFIG ${VERSION_CLI11} REQUIRED)
else()
  set(JSON_ImplicitConversions OFF)
  FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v${VERSION_NLOHMANN_JSON}/json.tar.xz)
  FetchContent_MakeAvailable(json)

  FetchContent_Declare(magic_enum URL https://github.com/Neargye/magic_enum/archive/refs/tags/v${VERSION_MAGIC_ENUM}.tar.gz)
  FetchContent_MakeAvailable(magic_enum)

  FetchContent_Declare(cli11 URL https://github.com/CLIUtils/CLI11/archive/refs/tags/v${VERSION_CLI11}.tar.gz)
  FetchContent_MakeAvailable(cli11)
endif() 

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
if (BUILD_FOR_LINUX)
  add_subdirectory(linux)
elseif (BUILD_FOR_WINDOWS)
  # Make public version of wil available.
  set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Turn off wil tests")
  set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Turn off wil packaging")

  if (USE_VCPKG)
    find_package(wil CONFIG REQUIRED)
  else()
    FetchContent_Declare(WIL
        GIT_REPOSITORY "https://github.com/microsoft/wil"
        GIT_TAG "5eb59d60e167482639cc47ffb40442158da7fd04"
        GIT_SHALLOW OFF
    )
    FetchContent_MakeAvailable(WIL)
  endif()

  # Add Windows-specific global compile definitions.
  add_compile_definitions(
      UNICODE
      NOMINMAX
      WIN32_LEAN_AND_MEAN
  )

  add_subdirectory(windows)
endif()

add_subdirectory(lib)
add_subdirectory(tests)
add_subdirectory(tools)
