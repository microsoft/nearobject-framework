add_subdirectory(devices)
add_subdirectory(nearobject)
add_subdirectory(nearobjectd)

if (NOT NOF_DISABLE_TOOLS)
  add_subdirectory(tools)
endif()
