
add_library(nearobject-service-linux STATIC "")

set(NO_SERVICE_LINUX_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_SERVICE_LINUX_DIR_PUBLIC_INCLUDE_PREFIX ${NO_SERVICE_LINUX_DIR_PUBLIC_INCLUDE}/linux/nearobject/service)

target_sources(nearobject-service-linux
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.cxx
    PUBLIC
        ${NO_SERVICE_LINUX_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceDiscoveryAgentUwb.hxx
)

target_include_directories(nearobject-service-linux
    PUBLIC
        ${NO_SERVICE_LINUX_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject-service-linux
    PRIVATE
        linuxdevuwb
    PUBLIC
        nearobject-service
)

set_target_properties(nearobject-service-linux PROPERTIES FOLDER linux/service)
