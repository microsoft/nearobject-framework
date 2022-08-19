
add_library(nearobject-service-linux STATIC "")

target_sources(nearobject-service-linux
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgentUwb.hxx
)

target_include_directories(nearobject-service-linux
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(nearobject-service-linux
    PRIVATE
        linuxdevuwb
    PUBLIC
        nearobject-service
)

set_target_properties(nearobject-service-linux PROPERTIES FOLDER linux/service)
