
add_library(windevuwb STATIC "")

target_sources(windevuwb
    PRIVATE
        UwbDevice.cxx
        UwbDevice.hxx
)

target_include_directories(windevuwb
    PRIVATE
        ${NOF_DIR_LIB}
)

set_target_properties(windevuwb PROPERTIES FOLDER windows/devices)
