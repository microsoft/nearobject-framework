
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        UwbDevice.cxx
        UwbRangingSession.cxx
        UwbSession.cxx
    PUBLIC
        UwbDevice.hxx
        UwbRangingSession.hxx
        UwbSession.hxx
)

target_link_libraries(uwb
    INTGERFACE
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
