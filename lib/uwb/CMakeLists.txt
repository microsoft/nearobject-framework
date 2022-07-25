
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

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
