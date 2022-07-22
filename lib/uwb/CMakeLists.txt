
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        UwbDevice.hxx
        UwbDevice.cxx
        UwbSession.cxx
        UwbSession.hxx
        UwbRangingSession.cxx
        UwbRangingSession.hxx
)

target_include_directories(uwb
    PRIVATE
        uci
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
