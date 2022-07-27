
add_library(smartcard STATIC "")

target_sources(smartcard
    PRIVATE
        Apdu.hxx
        Apdu.cxx
        Smartcard.cxx
        Smartcard.hxx
)

target_include_directories(smartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared/tlv
)

target_link_libraries(smartcard
    PUBLIC
        GSL
        no_tlv
)

set_target_properties(smartcard PROPERTIES FOLDER lib/smartcard)
