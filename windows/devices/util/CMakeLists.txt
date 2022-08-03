
add_library(windevutil STATIC "")

target_sources(windevutil
    PRIVATE
        WindowsDeviceEnumerator.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/WindowsDeviceEnumerator.hxx
        ${CMAKE_CURRENT_LIST_DIR}/WindowsDeviceResource.hxx
)

target_include_directories(windevutil
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(windevutil
    PUBLIC
        WIL
        cfgmgr32.lib
)

set_target_properties(windevutil PROPERTIES FOLDER windows/devices)

