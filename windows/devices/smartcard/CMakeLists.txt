
add_library(windows-devices-smartcard STATIC "")

set(SMARTCARD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX ${SMARTCARD_DIR_PUBLIC_INCLUDE}/windows/smartcard)

target_sources(windows-devices-smartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Smartcard.cxx
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

target_include_directories(windows-devices-smartcard
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windows-devices-smartcard
    PRIVATE
        smartcard
        plog::plog
)

list(APPEND SMARTCARD_PUBLIC_HEADERS
    ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

set_target_properties(windows-devices-smartcard PROPERTIES FOLDER windows/devices/smartcard)

set_target_properties(windows-devices-smartcard PROPERTIES PUBLIC_HEADER "${SMARTCARD_PUBLIC_HEADERS}")

install(
    TARGETS windows-devices-smartcard
    EXPORT smartcard
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/smartcard
)
