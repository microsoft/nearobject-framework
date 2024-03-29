cmake_minimum_required(VERSION 3.25)

# Project configuration options.
option(NOF_USE_MSVC_STATIC_RUNTIME, "Statically link against the VC++ runtime library" OFF)
option(NOF_CODE_COVERAGE "Enable instrumenting the build with code coverage" OFF)
option(NOF_BUILD_WINDOWS_UWB_SIMULATOR_DRIVER "Enable building the Windows UWB Simulator driver" OFF)
option(NOF_OFFICIAL_RELEASE "Enable building the project for an official release" OFF)

# Build environment definitions.
# 
# When "development" is selected, the project is configured to build for active development purposes.
# When "vcpkg" is selected, the project is configured to build on behalf of a vcpkg port.
#
set(NOF_BUILD_ENVIRONMENT_DEVELOPMENT "development")
set(NOF_BUILD_ENVIRONMENT_VCPKG "vcpkg")
set(NOF_BUILD_ENVIRONMENT ${NOF_BUILD_ENVIRONMENT_DEVELOPMENT} CACHE STRING "Build profile: either local 'development' or 'vcpkg'")

include(cmake/vcpkg.cmake)

# vcpkg feature related variables and options.
#
# These exist solely for being set through the vcpkg port file and each has a
# 1:1 mapping to all official vcpkg features for our package. The following
# table describes the mapping between feature name and CMake variable:
#
# vcpkg feature name       |    CMake variable name                  
# -------------------------|-----------------------------------------
# <top-level feature>      | NOF_VCPKG_ENABLE
# msvc-link-static-runtime | NOF_VCPKG_FEATURE_USE_MSVC_STATIC_RUNTIME
# enable-development       | NOF_VCPKG_FEATURE_ENABLE_DEVELOPMENT
#
option(NOF_VCPKG_ENABLE "Enable building the project via a vcpkg port" OFF)
option(NOF_VCPKG_FEATURE_USE_MSVC_STATIC_RUNTIME "vcpkg feature: Statically link against the VC++ runtime library" OFF)
option(NOF_VCPKG_FEATURE_ENABLE_DEVELOPMENT "vcpkg feature: Enable development mode, using the latest sources" OFF)

# vcpkg build target.
# 
# This controls the versioning information used when the vcpkg is built. 
# When "development" is selected, the HEAD revision of the current branch will be used.  
# When "release" is selected, the latest hard-coded version in packaging/vcpkg/CMakeLists.txt.
#
set(NOF_VCPKG_TARGET_DEVELOPMENT "development")
set(NOF_VCPKG_TARGET_RELEASE "release")
set(NOF_VCPKG_TARGET ${NOF_VCPKG_TARGET_DEVELOPMENT} CACHE STRING "vcpkg target: either 'development' or 'release'") 

# vcpkg const variables.
set(VCPKG_MANIFEST_DIR ${CMAKE_CURRENT_LIST_DIR}/packaging/vcpkg/ports/nearobject-framework)
set(NOF_VCPKG_SUBMODULE_ROOT ${CMAKE_CURRENT_LIST_DIR}/packaging/vcpkg/vcpkg)

# Enable vcpkg build environment if building on behalf of vcpkg.
if (NOF_VCPKG_ENABLE)
  set(NOF_BUILD_ENVIRONMENT ${NOF_BUILD_ENVIRONMENT_VCPKG} CACHE STRING "override" FORCE)
endif()

# Configure vcpkg to resolve dependencies.
vcpkg_configure(SUBMODULE_ROOT ${NOF_VCPKG_SUBMODULE_ROOT})

# Apply build-environment specific settings and configuration.
if (NOF_BUILD_ENVIRONMENT STREQUAL NOF_BUILD_ENVIRONMENT_VCPKG)
  # Enable static runtime linking if requested via vcpkg feature.
  if (NOF_VCPKG_FEATURE_USE_MSVC_STATIC_RUNTIME)
    set(NOF_USE_MSVC_STATIC_RUNTIME TRUE CACHE BOOL "override" FORCE)
  endif()
  # Enable development target if requested.
  if (NOF_VCPKG_FEATURE_ENABLE_DEVELOPMENT)
    set(NOF_VCPKG_TARGET ${NOF_VCPKG_TARGET_DEVELOPMENT} CACHE STRING "override") 
  endif()
elseif (NOF_BUILD_ENVIRONMENT STREQUAL NOF_BUILD_ENVIRONMENT_DEVELOPMENT)
  # Nothing special needed for the development build environment.
else()
  MESSAGE(FATAL_ERROR "Unknown build environment '${NOF_BUILD_ENVIRONMENT}' specified")
endif()

MESSAGE(STATUS "using build environment '${NOF_BUILD_ENVIRONMENT}'")

project(nearobject-framework 
  LANGUAGES CXX
  VERSION 0.5.4
)

# Conditional inclusion of OS-dependent source trees.
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(BUILD_FOR_LINUX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(BUILD_FOR_WINDOWS TRUE)
else ()
  MESSAGE(FATAL_ERROR "No supported target OS detected, SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
endif()

MESSAGE(STATUS "Target OS ${CMAKE_SYSTEM_NAME} detected")

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

include(CheckPIESupported)
include(CTest)
include(GNUInstallDirs)
include(cmake/cpack.cmake)

# Set external dependency target versions.
set(VERSION_NLOHMANN_JSON 3.11.2)
set(VERSION_MAGIC_ENUM 0.8.1)
set(VERSION_CLI11 2.2.0)
set(VERSION_CATCH2 3.1.0)
set(VERSION_PLOG 1.1.9)

# Pull in external dependencies.
set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
find_package(nlohmann_json CONFIG ${VERSION_NLOHMANN_JSON} REQUIRED)
find_package(magic_enum CONFIG ${VERSION_MAGIC_ENUM} REQUIRED)
find_package(CLI11 CONFIG ${VERSION_CLI11} REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(Threads REQUIRED)

# Uncomment for verbose build information
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Use folders. In generated projects that support project organization, this
# allows specifying the 'FOLDER' property on a target, which will create a
# corresponding folder in the project. This is useful for MSBuild generated
# projects, where these folders will appear in IDEs that interpret them (eg.
# Visual Studio).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable POSITION_INDEPENDENT_CODE variable to control passing PIE flags to the linker.
if (POLICY CMP0083)
  cmake_policy(SET CMP0083 NEW)
endif()

# Enable position independent executables.
check_pie_supported(LANGUAGES CXX)
if (CMAKE_CXX_LINK_PIE_SUPPORTED)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if (WERROR)
  add_compile_options(-Werror)
endif()

# Set compiler specific options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Common options for both gcc and clang
  add_compile_options(
    -fstack-protector-all
    -fvisibility=hidden
    -fcf-protection
    -fpermissive
    -mshstk
    -Wall 
    -Wshadow
    -Wformat-security
    -Wextra
    -Wpedantic
    -Wconversion
    -Walloca
    -Wvla
  )

  # gcc specific options
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(
      -fstack-clash-protection
      -Wtrampolines
      -Wl,-z,noexecstack
      -Wl,-z,now
      -Wl,-z,relro
      -z noexecstack
    )
    if (NOF_CODE_COVERAGE)
      add_compile_options(
        -fprofile-arcs
        -ftest-coverage
      )
    endif()
  # clang specific options
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if (NOF_CODE_COVERAGE)
      add_compile_options(
        -fprofile-instr-generate
        -fcoverage-mapping
      )
    endif()
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # TODO
endif()

if (CMAKE_BUILD_TYPE MATCHES "(Release|RelWithDebInfo|MinSizeRel)")
  add_compile_definitions(_FORTIFY_SOURCE=2)
endif()

# Common source directories
set(NOF_DIR_LIB ${CMAKE_CURRENT_LIST_DIR}/lib)
set(NOF_DIR_LINUX ${CMAKE_CURRENT_LIST_DIR}/linux)
set(NOF_DIR_WINDOWS ${CMAKE_CURRENT_LIST_DIR}/windows)

# Add common, global compile definitions.
add_compile_definitions(
    MAGIC_ENUM_RANGE_MIN=0
    MAGIC_ENUM_RANGE_MAX=255
)

# Conditional inclusion of OS-dependent source trees.
if (BUILD_FOR_LINUX)
  add_subdirectory(linux)
elseif (BUILD_FOR_WINDOWS)
  # Define Windows-specific package dependency versions.
  set(VERSION_CPPWINRT 2.0.230225.1)
  set(CPPWINRT_BUILD_VERSION ${VERSION_CPPWINRT})

  # Make public version of wil available.
  set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Turn off wil tests")
  set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Turn off wil packaging")

  find_package(wil CONFIG REQUIRED)

  if (NOT NOF_VCPKG_ENABLE)
    # Find nuget and restore packages for this project.
    # Get nuget program (assumes it is on PATH).
    find_program(nuget NAMES nuget REQUIRED)

    # Configure the nuget packages configuration with variable substitutions.
    configure_file(
      ${CMAKE_CURRENT_LIST_DIR}/packages.config.in
      ${CMAKE_CURRENT_LIST_DIR}/packages.config
    )

    # Restore nuget package dependencies.
    include(cmake/cppwinrt.cmake)
    include(cmake/nuget.cmake)
    restore_nuget_packages()
  endif()

  # Add Windows-specific global compile definitions.
  add_compile_definitions(
      _UNICODE
      UNICODE
      NOMINMAX
      WIN32_LEAN_AND_MEAN
  )

  # Allow more than default number of sections in object files.
  add_compile_options(
    /bigobj
  )

  if (NOF_USE_MSVC_STATIC_RUNTIME)
    # Enable explicit selection of MSVC runtime library.
    if (POLICY CMP0091)
      cmake_policy(SET CMP0091 NEW)
      # Statically link to the VC++ runtime library. This is required because
      # when an MSBuild project targets the 'WindowsUserModeDriver10.0' toolset,
      # the runtime library settings are completely ignored and instead links
      # staticaly to the VC++ runtime and dynamically to the UCRT. See the
      # following page for aditional details: more details:
      # https://learn.microsoft.com/en-us/windows-hardware/drivers/develop/using-the-microsoft-c-runtime-with-user-mode-drivers-and-apps
      MESSAGE(STATUS "Enabled static linking to VC++ runtime")
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
  endif()

  add_subdirectory(windows)
endif()

add_subdirectory(external)
add_subdirectory(lib)
add_subdirectory(packaging)

if (NOT NOF_DISABLE_TOOLS)
  add_subdirectory(tools)
endif()

if (NOT NOF_DISABLE_TESTS)
  add_subdirectory(tests)
endif()
