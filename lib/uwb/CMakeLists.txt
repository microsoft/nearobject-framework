
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        IUwbDevice.hxx
        UwbDevice.hxx
        UwbDevice.cxx
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)

add_subdirectory(uci)
