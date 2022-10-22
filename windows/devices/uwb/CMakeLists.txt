
add_library(windevuwb STATIC "")

set(WINDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWB_DIR_PUBLIC_INCLUDE}/windows/uwb)

target_sources(windevuwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
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
        uwbcx
    PUBLIC
        cfgmgr32.lib
        uwb
        WIL
)

set_target_properties(windevuwb PROPERTIES FOLDER windows/devices)
