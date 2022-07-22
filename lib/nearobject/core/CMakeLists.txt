
add_library(nearobject-core STATIC "")

target_sources(nearobject-core
    PRIVATE
        ConnectionProfile.cxx
        ConnectionProfile.hxx
        ConnectionProfileManager.cxx
        ConnectionProfileManager.hxx
        DeviceManager.hxx
        NearObject.hxx
        NearObject.cxx
        OobChannel.cxx
        OobChannel.hxx
        OobChannelBluetooth.cxx
        OobChannelBluetooth.hxx
        OobConnection.cxx
        OobConnection.hxx
        OobManager.cxx
        OobManager.hxx
        SecureDeviceManager.hxx
        ISecureDevice.hxx
        ISecureDeviceChannel.hxx
        SecureDevice.cxx
        SecureDevice.hxx
)

target_include_directories(nearobject-core
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${NOF_DIR_LIB}/se
        ${NOF_DIR_LIB}/se/globalplatform
        ${NOF_DIR_LIB}/smartcard
        ${NOF_DIR_LIB}/uwb
        ${NOF_DIR_LIB}/uwb/uci
)

target_link_libraries(nearobject-core
    PRIVATE
        se
        seglobalplatform
        smartcard
        uwb
)

set_target_properties(nearobject-core PROPERTIES FOLDER lib/nearobject/core)
