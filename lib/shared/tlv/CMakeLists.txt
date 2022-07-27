
add_library(tlv STATIC "")

target_sources(tlv
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Tlv.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/TlvBer.hxx
    PRIVATE
        Tlv.cxx
        TlvBer.cxx
)

target_link_libraries(tlv
    PRIVATE
        GSL
)

set_target_properties(tlv PROPERTIES FOLDER lib/shared/tlv)
