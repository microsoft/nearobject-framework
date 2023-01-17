
add_library(uwbcx-driver INTERFACE)

target_sources(uwbcx-driver
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbCxLrpDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbCxLrpDeviceGlue.h
)

target_include_directories(uwbcx-driver
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)
