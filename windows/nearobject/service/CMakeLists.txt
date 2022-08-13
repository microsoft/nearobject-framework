
add_library(nearobject-service-windows STATIC "")

target_sources(nearobject-service-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.hxx
)

target_include_directories(nearobject-service-windows
    PRIVATE
        ${NOF_DIR_LIB}
        ${NOF_DIR_LIB}/shared
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(nearobject-service-windows
    PRIVATE
        windevuwb
    PUBLIC
        cfgmgr32.lib
        nearobject-service
        WIL
        windevutil
)

set_target_properties(nearobject-service-windows PROPERTIES FOLDER windows/service)
