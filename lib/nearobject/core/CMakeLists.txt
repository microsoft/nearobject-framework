
add_library(nearobject-core STATIC "")

target_sources(nearobject-core
    PRIVATE
        OobManager.cxx
        OobManager.hxx
        ProfileManager.hxx
        SecureDeviceManager.hxx
        UwbDeviceManager.hxx
)

target_include_directories(nearobject-core
    PRIVATE
        ${UWB_DIR_LIB}/se
        ${UWB_DIR_LIB}/se/globalplatform
        ${UWB_DIR_LIB}/smartcard
        ${UWB_DIR_LIB}/secureproximity
        ${UWB_DIR_LIB}/uwb
        ${UWB_DIR_LIB}/uwb/uci
)

target_link_libraries(nearobject-core
    PRIVATE
        se
        seglobalplatform
        smartcard
        uwb
        uwbuci
)

set_target_properties(nearobject-core PROPERTIES FOLDER lib/nearobject/core)
