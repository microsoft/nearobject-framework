add_library(uwbcxadapter STATIC "")

set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(uwbcxadapter
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbCxAdapter.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbAppConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbCxAdapterDdiLrp.cxx
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbAppConfiguration.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapter.hxx
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
)

target_include_directories(uwbcxadapter
    PUBLIC
        ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(uwbcxadapter
    PRIVATE
        windevutil
    PUBLIC
        cfgmgr32.lib
        uwb
        uwb-proto-fira
        uwbcx-driver
        WIL::WIL
)

list(APPEND UWBCXADAPTER_PUBLIC_HEADERS
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbAppConfiguration.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapter.hxx
    ${UWBCXADAPTER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxAdapterDdiLrp.hxx
)

set_target_properties(uwbcxadapter PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(uwbcxadapter PROPERTIES PUBLIC_HEADER "${UWBCXADAPTER_PUBLIC_HEADERS}")

install(
    TARGETS uwbcxadapter 
    EXPORT uwbcxadapter
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
        uwbcxadapter
        windevutil
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
