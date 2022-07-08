
add_library(smartcard STATIC "")

target_sources(smartcard
    PRIVATE
        Apdu.hxx
        Apdu.cxx
        ISmartCard.hxx
        Tlv.hxx
        TlvBer.hxx
        TlvBer.cxx
)

target_link_libraries(smartcard
    PUBLIC
        GSL
)

set_target_properties(smartcard PROPERTIES FOLDER lib/smartcard)
