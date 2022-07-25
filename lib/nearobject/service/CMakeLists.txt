
add_library(nearobject-service STATIC "")

target_sources(nearobject-service
    PRIVATE
        ConnectionProfileManager.cxx
        ConnectionProfileManager.hxx
        DeviceManager.hxx
        OobChannel.cxx
        OobChannel.hxx
        OobChannelBluetooth.cxx
        OobChannelBluetooth.hxx
        OobConnection.cxx
        OobConnection.hxx
        OobManager.cxx
        OobManager.hxx
        SecureDevice.cxx
        SecureDevice.hxx
        SecureDeviceChannel.hxx
        SecureDeviceManager.hxx
        Service.cxx
        Service.hxx
)

target_include_directories(nearobject-service
    PRIVATE
        ${NOF_DIR_LIB}/nearobject/core
)

target_link_libraries(nearobject-service
    PRIVATE
        Threads::Threads
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
