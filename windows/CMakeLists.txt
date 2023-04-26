add_subdirectory(devices)
add_subdirectory(drivers)
add_subdirectory(nearobject)
add_subdirectory(shared)

if (NOT NOF_DISABLE_TOOLS)
  add_subdirectory(tools)
endif()
