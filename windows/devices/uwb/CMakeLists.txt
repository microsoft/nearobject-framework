
add_library(windevuwb STATIC "")

target_sources(windevuwb
    PRIVATE
        UwbDevice.cxx
        UwbDevice.hxx
)

set_target_properties(windevuwb PROPERTIES FOLDER windows/devices)
