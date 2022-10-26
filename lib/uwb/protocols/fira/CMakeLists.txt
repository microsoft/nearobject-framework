add_library(uwb-proto-fira)

set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE ${UWB_DIR_PUBLIC_INCLUDE})
set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb/protocols/fira)

target_sources(uwb-proto-fira
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControleePreference.cxx
        ${CMAKE_CURRENT_LIST_DIR}/FiraDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/RangingConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbCapability.cxx
    PUBLIC
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/ControleePreference.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/FiraDevice.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/RegulatoryInformation.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/RangingConfiguration.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/SecureRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/StaticRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCapability.hxx
)

target_include_directories(uwb-proto-fira
    PUBLIC
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(uwb-proto-fira
    PRIVATE
        uwb-proto-fira-uci
    PUBLIC
        smartcard
        tlv
        uwb
)

set_target_properties(uwb-proto-fira PROPERTIES FOLDER lib/uwb/protocol/fira)

add_subdirectory(uci)
