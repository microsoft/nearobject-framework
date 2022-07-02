
add_library(seiso STATIC "")

target_sources(seiso
    PRIVATE
        Apdu.hxx
        Apdu.cxx
        Tlv.hxx
        TlvBer.hxx
        TlvBer.cxx
)

set_target_properties(seiso PROPERTIES FOLDER lib/se)
