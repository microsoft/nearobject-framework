
add_library(nearobject-discovery STATIC "")

target_sources(nearobject-discovery
    PRIVATE
        NearObjectDiscoveryService.cxx
        NearObjectDiscoveryService.hxx
        NearObjectDiscoverySource.cxx
        NearObjectDiscoverySource.hxx
        NearObjectDiscoverySourceBluetooth.cxx
        NearObjectDiscoverySourceBluetooth.hxx
        NearObjectOobConnection.cxx
        NearObjectOobConnection.hxx
)

target_include_directories(nearobject-discovery
    PRIVATE
        ${NOF_DIR_LIB}
        ${NOF_DIR_LIB}/shared
)

target_link_libraries(nearobject-discovery
    PRIVATE
        nearobject-service
)

set_target_properties(nearobject-discovery PROPERTIES FOLDER lib/nearobject/service)
