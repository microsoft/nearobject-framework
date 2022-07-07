
add_library(uwbuci STATIC "")

target_sources(uwbuci
    PRIVATE
        ControlMessage.cxx
        ControlMessage.hxx
        ControlPacket.hxx
)

set_target_properties(uwbuci PROPERTIES FOLDER lib/uwb/uci)
