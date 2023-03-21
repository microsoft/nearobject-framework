add_library(windev-uwb-cx-adapter STATIC "")

set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windev-uwb-cx-adapter
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbCxAdapterDdiLrp.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceConnector.cxx
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbDeviceDdi.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceConnector.hxx
)

target_include_directories(windev-uwb-cx-adapter
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windev-uwb-cx-adapter
    PRIVATE
        windev-util
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
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceConnector.hxx
)

set_target_properties(windev-uwb-cx-adapter PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(windev-uwb-cx-adapter PROPERTIES PUBLIC_HEADER "${UWBCXADAPTER_PUBLIC_HEADERS}")

install(
    TARGETS windev-uwb-cx-adapter 
    EXPORT windev-uwb-cx-adapter
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb
)

add_library(windev-uwb STATIC "")

set(WINDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWB_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windev-uwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

target_include_directories(windev-uwb
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windev-uwb
    PRIVATE
        windev-util
        windev-uwb-cx-adapter
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

set_target_properties(windev-uwb PROPERTIES FOLDER windows/devices)
set_target_properties(windev-uwb PROPERTIES PUBLIC_HEADER "${WINDEVUWB_PUBLIC_HEADERS}")

install(
    TARGETS windev-uwb 
    EXPORT uwb
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb
)
