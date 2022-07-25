
add_library(fira-uci STATIC "")

target_sources(fira-uci
    PRIVATE
        ControlMessage.cxx
        ControlMessage.hxx
        ControlPacket.hxx
)

set_target_properties(fira-uci PROPERTIES FOLDER lib/fira)
