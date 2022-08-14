
add_library(nearobject-service STATIC "")

set(NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/service)

target_sources(nearobject-service
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceDiscoveryAgent.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceUwb.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfilePersisterFilesystem.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfilePersisterFilesystem.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/ServiceRuntime.cxx
    PUBLIC
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDevice.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceDiscoveryAgent.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceManager.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceUwb.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileManager.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfilePersister.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectService.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectServiceInjector.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureDevice.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureDeviceChannel.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/ServiceRuntime.hxx
)

target_link_libraries(nearobject-service
    PRIVATE
        notstd
        Threads::Threads
        uwb
    PUBLIC
        jsonify
        nearobject
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
