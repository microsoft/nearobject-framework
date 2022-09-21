
add_library(nearobject-proto-fira-uci STATIC "")

set(NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE ${NO_DIR_PUBLIC_INCLUDE})
set(NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE}/nearobject/protocols/fira/uci)

target_sources(nearobject-proto-fira-uci
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControlMessage.cxx
    PUBLIC
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlMessage.hxx
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlPacket.hxx
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceState.hxx
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/SessionState.hxx
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/StatusCodes.hxx
)

target_include_directories(nearobject-proto-fira-uci
    PUBLIC
        ${NO_PROT_FIRA_UCI_DIR_PUBLIC_INCLUDE}
)

set_target_properties(nearobject-proto-fira-uci PROPERTIES FOLDER lib/nearobject/protocol/fira)
