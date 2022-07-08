
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        OobChannel.cxx
        OobChannel.hxx
        OobChannelBluetooth.cxx
        OobChannelBluetooth.hxx
        OobConnection.cxx
        OobConnection.hxx
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

target_link_libraries(uwb
    PRIVATE
        uwbuci
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)

add_subdirectory(uci)
