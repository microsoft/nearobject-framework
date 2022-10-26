
add_library(uwbcx INTERFACE)

target_sources(uwbcx
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbCxLrpDevice.h
        ${CMAKE_CURRENT_LIST_DIR}/include/UwbCxLrpDeviceGlue.h
)

target_include_directories(uwbcx
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)
