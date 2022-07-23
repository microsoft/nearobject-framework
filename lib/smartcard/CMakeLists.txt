
add_library(smartcard STATIC "")

target_sources(smartcard
    PRIVATE
        Apdu.hxx
        Apdu.cxx
        ISmartcard.cxx
        ISmartcard.hxx
        Tlv.hxx
        TlvBer.hxx
        TlvBer.cxx
)

target_include_directories(smartcard
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(smartcard
    PUBLIC
        GSL
)

set_target_properties(smartcard PROPERTIES FOLDER lib/smartcard)
