
add_library(nearobject-discovery STATIC "")

target_sources(nearobject-discovery
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoveryService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySource.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySourceBluetooth.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectOobConnection.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoveryService.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySource.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDiscoverySourceBluetooth.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectOobConnection.hxx
)

target_link_libraries(nearobject-discovery
    PRIVATE
        nearobject-service
)

set_target_properties(nearobject-discovery PROPERTIES FOLDER lib/nearobject/service)
