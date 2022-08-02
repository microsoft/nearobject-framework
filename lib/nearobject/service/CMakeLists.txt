
add_library(nearobject-service STATIC "")

target_sources(nearobject-service
    PRIVATE
        NearObjectDevice.cxx
        NearObjectDevice.hxx
        NearObjectDeviceDiscoveryAgent.cxx
        NearObjectDeviceDiscoveryAgent.hxx
        NearObjectDeviceManager.cxx
        NearObjectDeviceManager.hxx
        NearObjectDeviceUwb.cxx
        NearObjectDeviceUwb.hxx
        NearObjectProfileManager.cxx
        NearObjectProfileManager.hxx
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
        ${NOF_DIR_LIB}/shared
)

target_link_libraries(nearobject-service
    PRIVATE
        nearobject
        Threads::Threads
        uwb
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
