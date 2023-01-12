
add_library(uwbsim INTERFACE)

target_sources(uwbsim
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbSimulatorDdi.h
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbSimulatorDdiGlue.h
)

target_include_directories(uwbsim
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)
