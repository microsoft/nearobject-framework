
add_library(nearobject-service STATIC "")

set(NO_SERVICE_DIR_PUBLIC_INCLUDE ${NO_DIR_PUBLIC_INCLUDE})
set(NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/service)

target_sources(nearobject-service
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceController.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceControllerDiscoveryAgent.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceControllerManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectDeviceControllerUwb.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectServiceConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectSessionIdGeneratorRandom.cxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/ServiceRuntime.cxx
    PUBLIC
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceController.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceControllerDiscoveryAgent.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceControllerManager.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectDeviceControllerUwb.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileManager.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectService.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectServiceConfiguration.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectServiceInjector.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSessionIdGenerator.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSessionIdGeneratorRandom.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureDevice.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureDeviceChannel.hxx
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE_PREFIX}/ServiceRuntime.hxx
)

target_include_directories(nearobject-service
    PUBLIC
        ${NO_SERVICE_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject-service
    PRIVATE
        CLI11::CLI11
        notstd
        Threads::Threads
        uwb
    PUBLIC
        jsonify
        nearobject
        nearobject-persist
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
