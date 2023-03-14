
add_library(windev-util STATIC "")

set(WINDEVUTIL_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUTIL_DIR_PUBLIC_INCLUDE}/windows/devices)

target_sources(windev-util
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/DeviceEnumerator.cxx
        ${CMAKE_CURRENT_LIST_DIR}/DeviceHandle.cxx
        ${CMAKE_CURRENT_LIST_DIR}/DevicePresenceMonitor.cxx
    PUBLIC
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceEnumerator.hxx
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceHandle.hxx
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DevicePresenceMonitor.hxx
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceResource.hxx
)

target_include_directories(windev-util
    PUBLIC
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windev-util
    PRIVATE
        plog::plog
    PUBLIC
        cfgmgr32.lib
        notstd-windows
        WIL::WIL
)

list(APPEND WINDEVUTIL_PUBLIC_HEADERS
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceEnumerator.hxx
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceHandle.hxx
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DevicePresenceMonitor.hxx
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceResource.hxx
)

set_target_properties(windev-util PROPERTIES FOLDER windows/devices)
set_target_properties(windev-util PROPERTIES PUBLIC_HEADER "${WINDEVUTIL_PUBLIC_HEADERS}")

install(
    TARGETS windev-util
    EXPORT windev-util
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices
)
