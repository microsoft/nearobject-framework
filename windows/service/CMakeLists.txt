
add_executable(winuwbservice "")

target_sources(winuwbservice
    PRIVATE
        Main.cxx
        OobManager.cxx
        OobManager.hxx
        ProfileManager.hxx
        SecureDeviceManager.hxx
        Service.hxx
        Service.cxx
        UwbDeviceManager.hxx
)

target_include_directories(winuwbservice
    PRIVATE
        ${UWB_DIR_LIB}/se
        ${UWB_DIR_LIB}/se/globalplatform
        ${UWB_DIR_LIB}/smartcard
        ${UWB_DIR_LIB}/secureproximity
        ${UWB_DIR_LIB}/uwb
        ${UWB_DIR_LIB}/uwb/uci
)

target_link_libraries(winuwbservice
    PRIVATE
        se
        seglobalplatform
        smartcard
        uwb
        uwbuci
)

set_target_properties(winuwbservice PROPERTIES FOLDER windows/service)
