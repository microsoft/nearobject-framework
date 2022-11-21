
add_library(nearobject-service-windows STATIC "")

set(NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE_PREFIX ${NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE}/windows/nearobject/service)

target_sources(nearobject-service-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.cxx
    PUBLIC
        ${NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceDiscoveryAgentUwb.hxx
)

target_include_directories(nearobject-service-windows
    PUBLIC
        ${NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject-service-windows
    PRIVATE
        windevuwb
    PUBLIC
        cfgmgr32.lib
        nearobject-service
        WIL::WIL
        windevutil
)

set_target_properties(nearobject-service-windows PROPERTIES FOLDER windows/service)
