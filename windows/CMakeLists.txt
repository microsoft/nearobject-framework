
# Make public version of wil available.
set(WIL_BUILD_TESTS OFF CACHE INTERNAL "Turn off wil tests")
set(WIL_BUILD_PACKAGING OFF CACHE INTERNAL "Turn off wil packaging")

if (NOF_USE_VCPKG)
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
  _UNICODE
  UNICODE
  NOMINMAX
  WIN32_LEAN_AND_MEAN
)

if (NOF_WINDOWS_BUILD_DRIVER)
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

add_subdirectory(devices)
add_subdirectory(drivers)
add_subdirectory(nearobject)
add_subdirectory(nearobjectsvc)
add_subdirectory(shared)
add_subdirectory(tools)
