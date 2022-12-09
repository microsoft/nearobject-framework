add_library(uwb-proto-fira STATIC "")

set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE ${UWB_DIR_PUBLIC_INCLUDE})
set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb/protocols/fira)

target_sources(uwb-proto-fira
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControleePreference.cxx
        ${CMAKE_CURRENT_LIST_DIR}/FiraDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/RangingConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbCapability.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSessionData.cxx
    PUBLIC
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/ControleePreference.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/FiraDevice.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/RangingConfiguration.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/SecureRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/StaticRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCapability.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConfiguration.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbRegulatoryInformation.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionData.hxx
)

target_include_directories(uwb-proto-fira
    PUBLIC
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(uwb-proto-fira
    PRIVATE
        uwb-proto-fira-uci
        magic_enum::magic_enum
    PUBLIC
        notstd
        smartcard
        tlv
        uwb
)

set_target_properties(uwb-proto-fira PROPERTIES FOLDER lib/uwb/protocol/fira)

add_subdirectory(uci)
