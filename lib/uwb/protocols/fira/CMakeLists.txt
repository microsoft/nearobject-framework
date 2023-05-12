add_library(uwb-proto-fira STATIC "")

set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE ${UWB_DIR_PUBLIC_INCLUDE})
set(UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb/protocols/fira)

target_sources(uwb-proto-fira
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ControleePreference.cxx
        ${CMAKE_CURRENT_LIST_DIR}/FiraDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/RangingMethod.cxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/StaticRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbCapability.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbConfigurationBuilder.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbOobConversions.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSessionData.cxx
    PUBLIC
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/ControleePreference.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/FiraDevice.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/RangingMethod.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/SecureRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/StaticRangingInfo.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCapability.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConfiguration.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConfigurationBuilder.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbException.hxx
        ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbOobConversions.hxx
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

list(APPEND UWBPROTOFIRA_PUBLIC_HEADERS
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/ControleePreference.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/FiraDevice.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/RangingMethod.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/SecureRangingInfo.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/StaticRangingInfo.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCapability.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConfiguration.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConfigurationBuilder.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbOobConversions.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbException.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbRegulatoryInformation.hxx
    ${UWB_PROTO_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionData.hxx
)

set_target_properties(uwb-proto-fira PROPERTIES FOLDER lib/uwb/protocols/fira)
set_target_properties(uwb-proto-fira PROPERTIES PUBLIC_HEADER "${UWBPROTOFIRA_PUBLIC_HEADERS}")

install(
    TARGETS uwb-proto-fira
    EXPORT uwb-proto-fira
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uwb/protocols/fira
)

add_subdirectory(uci)
