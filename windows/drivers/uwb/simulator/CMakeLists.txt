
add_library(uwbsim-driver INTERFACE)

target_sources(uwbsim-driver
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbSimulatorDdi.h
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbSimulatorDdiGlue.h
)

target_include_directories(uwbsim-driver
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Build the simulator driver if enabled.
if (NOF_BUILD_WINDOWS_UWB_SIMULATOR_DRIVER)
  include_external_msproject(uwbsim-driver-msbuild 
    ${CMAKE_CURRENT_LIST_DIR}/UwbSimulator.sln
    uwb
    uwbcxadapter
    windev-uwb
  )
endif()
