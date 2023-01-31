
add_library(windevutil STATIC "")

set(WINDEVUTIL_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUTIL_DIR_PUBLIC_INCLUDE}/windows/devices)

target_sources(windevutil
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/DeviceEnumerator.cxx
        ${CMAKE_CURRENT_LIST_DIR}/DevicePresenceMonitor.cxx
    PUBLIC
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceEnumerator.hxx
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DevicePresenceMonitor.hxx
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceResource.hxx
)

target_include_directories(windevutil
    PUBLIC
        ${WINDEVUTIL_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windevutil
    PRIVATE
        plog::plog
    PUBLIC
        cfgmgr32.lib
        notstd-windows
        WIL::WIL
)

list(APPEND WINDEVUTIL_PUBLIC_HEADERS
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceEnumerator.hxx
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DevicePresenceMonitor.hxx
    ${WINDEVUTIL_DIR_PUBLIC_INCLUDE_PREFIX}/DeviceResource.hxx
)

set_target_properties(windevutil PROPERTIES FOLDER windows/devices)
set_target_properties(windevutil PROPERTIES PUBLIC_HEADER "${WINDEVUTIL_PUBLIC_HEADERS}")

install(
    TARGETS windevutil
    EXPORT windevutil
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices
)
