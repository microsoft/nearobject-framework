
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
