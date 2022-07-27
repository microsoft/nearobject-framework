
add_library(nearobject-service STATIC "")

target_sources(nearobject-service
    PRIVATE
        NearObjectConnectionProfileManager.cxx
        NearObjectConnectionProfileManager.hxx
        NearObjectDevice.cxx
        NearObjectDevice.hxx
        NearObjectDeviceManager.cxx
        NearObjectDeviceManager.hxx
        NearObjectDeviceUwb.cxx
        NearObjectDeviceUwb.hxx
        NearObjectDiscoveryService.cxx
        NearObjectDiscoveryService.hxx
        NearObjectDiscoverySource.cxx
        NearObjectDiscoverySource.hxx
        NearObjectDiscoverySourceBluetooth.cxx
        NearObjectDiscoverySourceBluetooth.hxx
        NearObjectOobConnection.cxx
        NearObjectOobConnection.hxx
        NearObjectService.cxx
        NearObjectService.hxx
        NearObjectServiceInjector.hxx
        SecureDevice.cxx
        SecureDevice.hxx
        SecureDeviceChannel.hxx
        ServiceRuntime.cxx
        ServiceRuntime.hxx
)

target_include_directories(nearobject-service
    PRIVATE
        ${NOF_DIR_LIB}
)

target_link_libraries(nearobject-service
    PRIVATE
        Threads::Threads
        nearobject
        uwb
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
