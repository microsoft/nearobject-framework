
add_library(windevuwb STATIC "")

target_sources(windevuwb
    PRIVATE
        UwbDevice.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.hxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceDriver.hxx
)

target_include_directories(windevuwb
    PRIVATE
        ${NOF_DIR_LIB}
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(windevuwb PROPERTIES FOLDER windows/devices)
