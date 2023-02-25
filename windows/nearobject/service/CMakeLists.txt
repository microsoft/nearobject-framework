
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
        windev-uwb
        plog::plog
    PUBLIC
        nearobject-service
        WIL::WIL
        windev-util
)

list(APPEND NO_SERVICE_WINDOWS_PUBLIC_HEADERS
    ${NO_SERVICE_WINDOWS_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceDiscoveryAgentUwb.hxx
)

set_target_properties(nearobject-service-windows PROPERTIES FOLDER windows/service)
set_target_properties(nearobject-service-windows PROPERTIES PUBLIC_HEADER "${NO_SERVICE_WINDOWS_PUBLIC_HEADERS}")

install(
    TARGETS nearobject-service-windows
    EXPORT nearobject
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/nearobject/service
)
