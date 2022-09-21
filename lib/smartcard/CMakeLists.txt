
add_library(smartcard STATIC "")

set(SMARTCARD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX ${SMARTCARD_DIR_PUBLIC_INCLUDE}/smartcard)

target_sources(smartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Apdu.cxx
        ${CMAKE_CURRENT_LIST_DIR}/Smartcard.cxx
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Apdu.hxx
        ${SMARTCARD_DIR_PUBLIC_INCLUDE_PREFIX}/Smartcard.hxx
)

target_include_directories(smartcard
    PUBLIC
        ${SMARTCARD_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(smartcard
    PUBLIC
        tlv
)

set_target_properties(smartcard PROPERTIES FOLDER lib/smartcard)
