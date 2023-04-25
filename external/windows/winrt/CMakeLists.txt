
set(WINRT_WINMD_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Copy the winmd file to the binary output directory.
configure_file(
  windows.devices.nearobject.winmd
  windows.devices.nearobject.winmd
  COPYONLY
)

# Create a MSBuild property sheet which references the winmd file with the proper path.
configure_file(
  windows.devices.nearobject.props.in
  windows.devices.nearobject.props
)

# Define target which depends on the winmd file.
add_custom_target(windows-devices-nearobject-winrt
  DEPENDS 
    ${WINRT_WINMD_DIR}/windows.devices.nearobject.winmd
    ${WINRT_WINMD_DIR}/windows.devices.nearobject.props
)

add_custom_command(OUTPUT
  windows.devices.nearobject.props
  COMMAND ${CMAKE_COMMAND} -E copy ${WINRT_WINMD_DIR}/windows.devices.nearobject.props
                                   ${CMAKE_CURRENT_BINARY_DIR}/windows.devices.nearobject.props
)
