
add_library(windevuwb STATIC "")

set(WINDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWB_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windevuwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbAppConfiguration.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

target_include_directories(windevuwb
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windevuwb
    PRIVATE
        windevutil
        magic_enum::magic_enum
    PUBLIC
        cfgmgr32.lib
        uwb
        uwb-proto-fira
        uwbcx
        WIL::WIL
)

list(APPEND WINDEVUWB_PUBLIC_HEADERS
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

set_target_properties(windevuwb PROPERTIES FOLDER windows/devices)
set_target_properties(windevuwb PROPERTIES PUBLIC_HEADER "${WINDEVUWB_PUBLIC_HEADERS}")

install(
    TARGETS windevuwb 
    EXPORT uwb
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/uwb
)

add_library(libuwbcxadapter STATIC "")

set(LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX ${LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(libuwbcxadapter
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbCxAdapter.cxx
    PUBLIC
        ${LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapter.hxx
)

target_include_directories(libuwbcxadapter
    PUBLIC
        ${LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(libuwbcxadapter
    PRIVATE
        windevutil
        magic_enum::magic_enum
    PUBLIC
        cfgmgr32.lib
        uwb
        uwb-proto-fira
        uwbcx
        WIL::WIL
)

list(APPEND LIBUWBCXADAPTER_PUBLIC_HEADERS
    ${LIBUWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapter.hxx
)

set_target_properties(libuwbcxadapter PROPERTIES FOLDER windows/devices)
set_target_properties(libuwbcxadapter PROPERTIES PUBLIC_HEADER "${WINDEVUWB_PUBLIC_HEADERS}")

install(
    TARGETS libuwbcxadapter 
    EXPORT uwb
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/uwb
)