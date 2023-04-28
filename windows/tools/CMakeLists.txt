
add_subdirectory(devicemon)
add_subdirectory(uwb)

if (NOT NOF_VCPKG_ENABLE)
  add_subdirectory(nocli)
endif()
