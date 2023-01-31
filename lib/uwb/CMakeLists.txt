
add_library(uwb STATIC "")

set(UWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWB_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb)

target_sources(uwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbMacAddress.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbPeer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbVersion.cxx
    PUBLIC
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbMacAddress.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbPeer.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionEventCallbacks.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbVersion.hxx
)

target_include_directories(uwb
    PUBLIC
        ${UWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(uwb
    PRIVATE
        magic_enum::magic_enum
        tlv
        plog::plog
    PUBLIC
        notstd
        uwb-proto-fira
)

list(APPEND UWB_PUBLIC_HEADERS
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbMacAddress.hxx
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbPeer.hxx
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionEventCallbacks.hxx
    ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbVersion.hxx
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
set_target_properties(uwb PROPERTIES PUBLIC_HEADER "${UWB_PUBLIC_HEADERS}")

install(
    TARGETS uwb
    EXPORT uwb
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/uwb
)

add_subdirectory(protocols)
