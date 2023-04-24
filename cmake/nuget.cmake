
# Calls nuget.exe restore on packages.config in the calling directory.
function (restore_nuget_packages)
  # Ensure that any changes to packages.config re-configures the project.
  set_property(DIRECTORY
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/packages.config)

    # Restore nuget packages
  MESSAGE(STATUS "restoring nuget packages")
  execute_process(COMMAND
    ${nuget} restore packages.config -SolutionDirectory ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND_ERROR_IS_FATAL ANY)
endfunction()
