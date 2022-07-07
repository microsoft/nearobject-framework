
add_library(uwbucicore STATIC "")

target_sources(uwbucicore
    PRIVATE
        ControlMessage.hxx
        ControlPacket.hxx
)

set_target_properties(uwbucicore PROPERTIES FOLDER lib/uwb/uci)
