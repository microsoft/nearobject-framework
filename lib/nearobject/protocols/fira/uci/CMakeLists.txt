
add_library(nearobject-proto-fira-uci STATIC "")

target_sources(nearobject-proto-fira-uci
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControlMessage.cxx
        ${CMAKE_CURRENT_LIST_DIR}/ControlMessage.hxx
        ${CMAKE_CURRENT_LIST_DIR}/ControlPacket.hxx
)

set_target_properties(nearobject-proto-fira-uci PROPERTIES FOLDER lib/nearobject/protocol/fira)
