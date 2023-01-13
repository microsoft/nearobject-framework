
add_library(windev-uwb STATIC "")

set(WINDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWB_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(windev-uwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
        ${WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbAppConfiguration.hxx
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
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/uwb
)
