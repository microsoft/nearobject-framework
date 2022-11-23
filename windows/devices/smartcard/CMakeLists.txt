
add_library(windevsmartcard STATIC "")

set(SMARTCARD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX ${SMARTCARD_DIR_PUBLIC_INCLUDE}/windows/smartcard)

target_sources(windevsmartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Smartcard.cxx
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

target_include_directories(windevsmartcard
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windevsmartcard
    PRIVATE
        smartcard
)

list(APPEND SMARTCARD_PUBLIC_HEADERS
    ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

set_target_properties(windevsmartcard PROPERTIES FOLDER windows/devices)

set_target_properties(windevsmartcard PROPERTIES PUBLIC_HEADER "${SMARTCARD_PUBLIC_HEADERS}")

install(
    TARGETS windevsmartcard
    EXPORT smartcard
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/smartcard
)
