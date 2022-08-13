
add_library(windevutil STATIC "")

target_sources(windevutil
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/DeviceEnumerator.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/DeviceEnumerator.hxx
        ${CMAKE_CURRENT_LIST_DIR}/DeviceResource.hxx
)

target_include_directories(windevutil
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(windevutil
    PUBLIC
        cfgmgr32.lib
        WIL
)

set_target_properties(windevutil PROPERTIES FOLDER windows/devices)
