
add_library(nearobject-discovery STATIC "")

set(NO_DISCOVERY_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/discovery)

target_sources(nearobject-discovery
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoveryService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySource.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySourceBluetooth.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectOobConnection.cxx
    PUBLIC
        ${NO_DISCOVERY_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDiscoveryService.hxx
        ${NO_DISCOVERY_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDiscoverySource.hxx
        ${NO_DISCOVERY_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDiscoverySourceBluetooth.hxx
        ${NO_DISCOVERY_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectOobConnection.hxx
)

target_link_libraries(nearobject-discovery
    PRIVATE
        nearobject-service
)

set_target_properties(nearobject-discovery PROPERTIES FOLDER lib/nearobject/service)
