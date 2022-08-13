
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

set_target_properties(windevsmartcard PROPERTIES FOLDER windows/devices)
