
add_library(smartcard STATIC "")

target_sources(smartcard
    PRIVATE
        Apdu.hxx
        Apdu.cxx
        Tlv.hxx
        TlvBer.hxx
        TlvBer.cxx
)

set_target_properties(smartcard PROPERTIES FOLDER lib/smartcard)
