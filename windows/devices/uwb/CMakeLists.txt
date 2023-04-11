add_library(windows-devices-uwb-cx-adapter STATIC "")

set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windows-devices-uwb-cx-adapter
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbCxAdapterDdiLrp.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbConnector.cxx
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbDeviceDdi.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbSessionDdi.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConnector.hxx
)

target_include_directories(windows-devices-uwb-cx-adapter
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windows-devices-uwb-cx-adapter
    PRIVATE
        windows-devices-util
        magic_enum::magic_enum
        synchronization.lib
    PUBLIC
        cfgmgr32.lib
        plog::plog
        uwb
        uwb-proto-fira
        uwbcx-driver
        WIL::WIL
)

list(APPEND UWBCXADAPTER_PUBLIC_HEADERS
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbDeviceDdi.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbSessionDdi.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbConnector.hxx
)

set_target_properties(windows-devices-uwb-cx-adapter PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(windows-devices-uwb-cx-adapter PROPERTIES PUBLIC_HEADER "${UWBCXADAPTER_PUBLIC_HEADERS}")

install(
    TARGETS windows-devices-uwb-cx-adapter 
    EXPORT windows-devices-uwb-cx-adapter
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb
)

add_library(windows-devices-uwb STATIC "")

set(WINDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWB_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windows-devices-uwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

target_include_directories(windows-devices-uwb
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windows-devices-uwb
    PRIVATE
        magic_enum::magic_enum
        windows-devices-util
        windows-devices-uwb-cx-adapter
    PUBLIC
        uwb
        uwb-proto-fira
        uwbcx-driver
        WIL::WIL
)

list(APPEND WINDEVUWB_PUBLIC_HEADERS
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
    ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

set_target_properties(windows-devices-uwb PROPERTIES FOLDER windows/devices)
set_target_properties(windows-devices-uwb PROPERTIES PUBLIC_HEADER "${WINDEVUWB_PUBLIC_HEADERS}")

install(
    TARGETS windows-devices-uwb 
    EXPORT uwb
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb
)
