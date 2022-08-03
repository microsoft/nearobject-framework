
add_library(tlv STATIC "")

target_sources(tlv
    PRIVATE
        TlvBer.cxx
        TlvSimple.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Tlv.hxx
        ${CMAKE_CURRENT_LIST_DIR}/TlvBer.hxx
        ${CMAKE_CURRENT_LIST_DIR}/TlvSimple.hxx
)

target_link_libraries(tlv
    PUBLIC 
        GSL
)

set_target_properties(tlv PROPERTIES FOLDER lib/shared/tlv)
