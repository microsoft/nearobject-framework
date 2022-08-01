
add_library(nearobject-service-windows STATIC "")

target_sources(nearobject-service-windows
    PRIVATE
        Main.cxx
        NearObjectDeviceDiscoveryAgentUwb.cxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectDeviceDiscoveryAgentUwb.cxx
)

target_include_directories(nearobject-service-windows
    PRIVATE
        ${NOF_DIR_LIB}
        ${NOF_DIR_LIB}/shared
)

target_link_libraries(nearobject-service-windows
    PRIVATE
        nearobject-service
        windevuwb
)

set_target_properties(nearobject-service-windows PROPERTIES FOLDER windows/service)
