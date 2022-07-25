
add_library(nearobject-proto-fira-uci STATIC "")

target_sources(nearobject-proto-fira-uci
    PRIVATE
        ControlMessage.cxx
        ControlMessage.hxx
        ControlPacket.hxx
)

set_target_properties(nearobject-proto-fira-uci PROPERTIES FOLDER lib/nearobject/protocol/fira)
