
add_library(uwb-proto-fira-uci STATIC "")

set(UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE ${UWB_DIR_PUBLIC_INCLUDE})
set(UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb/protocols/fira/uci)

target_sources(uwb-proto-fira-uci
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControlMessage.cxx
    PUBLIC
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlMessage.hxx
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlPacket.hxx
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceState.hxx
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/SessionState.hxx
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/StatusCodes.hxx
)

target_include_directories(uwb-proto-fira-uci
    PUBLIC
        ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE}
)

list(APPEND UWBPROTOFIRAUCI_PUBLIC_HEADERS
    ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlMessage.hxx
    ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/ControlPacket.hxx
    ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceState.hxx
    ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/SessionState.hxx
    ${UWB_PROTO_FIRA_UCI_DIR_PUBLIC_INCLUDE_PREFIX}/StatusCodes.hxx
)

set_target_properties(uwb-proto-fira-uci PROPERTIES FOLDER lib/uwb/protocols/fira)
set_target_properties(uwb-proto-fira-uci PROPERTIES PUBLIC_HEADER "${UWBPROTOFIRAUCI_PUBLIC_HEADERS}")

install(
    TARGETS uwb-proto-fira-uci
    EXPORT uwb-proto-fira-uci
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uwb/protocols/fira
)
