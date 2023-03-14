
add_library(windev-smartcard STATIC "")

set(SMARTCARD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX ${SMARTCARD_DIR_PUBLIC_INCLUDE}/windows/smartcard)

target_sources(windev-smartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Smartcard.cxx
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

target_include_directories(windev-smartcard
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windev-smartcard
    PRIVATE
        smartcard
        plog::plog
)

list(APPEND SMARTCARD_PUBLIC_HEADERS
    ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

set_target_properties(windev-smartcard PROPERTIES FOLDER windows/devices/smartcard)

set_target_properties(windev-smartcard PROPERTIES PUBLIC_HEADER "${SMARTCARD_PUBLIC_HEADERS}")

install(
    TARGETS windev-smartcard
    EXPORT smartcard
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/smartcard
)
