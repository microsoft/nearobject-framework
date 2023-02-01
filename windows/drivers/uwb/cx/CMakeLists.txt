
add_library(uwbcx-driver INTERFACE)

set(UWBCXDRIVER_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWBCXDRIVER_DIR_PUBLIC_INCLUDE_PREFIX ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE}/windows/devices/uwb)

target_sources(uwbcx-driver
    INTERFACE
        ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxDdiLrp.hxx
        ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxLrpDevice.h
)

target_include_directories(uwbcx-driver
    INTERFACE 
        ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE}
)

list(APPEND UWBCXDRIVER_PUBLIC_HEADERS
    ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxDdiLrp.hxx
    ${UWBCXDRIVER_DIR_PUBLIC_INCLUDE_PREFIX}/UwbCxLrpDevice.h
)

set_target_properties(uwbcx-driver PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(uwbcx-driver PROPERTIES PUBLIC_HEADER "${UWBCXDRIVER_PUBLIC_HEADERS}")

install(
    TARGETS uwbcx-driver 
    EXPORT uwbcx-driver
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb
)
