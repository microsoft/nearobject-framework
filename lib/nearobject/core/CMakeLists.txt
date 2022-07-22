
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
        ${NOF_DIR_LIB}/se
        ${NOF_DIR_LIB}/se/globalplatform
        ${NOF_DIR_LIB}/smartcard
        ${NOF_DIR_LIB}/secureproximity
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
