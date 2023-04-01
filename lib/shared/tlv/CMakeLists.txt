
add_library(tlv STATIC "")

set(TLV_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(tlv
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/TlvBer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TlvSimple.cxx
    PUBLIC
        ${TLV_DIR_PUBLIC_INCLUDE}/Tlv.hxx
        ${TLV_DIR_PUBLIC_INCLUDE}/TlvBer.hxx
        ${TLV_DIR_PUBLIC_INCLUDE}/TlvSimple.hxx
)

target_include_directories(tlv
    PUBLIC
        ${TLV_DIR_PUBLIC_INCLUDE}
)

list(APPEND TLV_PUBLIC_HEADERS
    ${TLV_DIR_PUBLIC_INCLUDE}/Tlv.hxx
    ${TLV_DIR_PUBLIC_INCLUDE}/TlvBer.hxx
    ${TLV_DIR_PUBLIC_INCLUDE}/TlvSimple.hxx
)

set_target_properties(tlv PROPERTIES FOLDER lib/shared/tlv)
set_target_properties(tlv PROPERTIES PUBLIC_HEADER "${TLV_PUBLIC_HEADERS}")

install(
    TARGETS tlv
    EXPORT tlv
    ARCHIVE
    PUBLIC_HEADER
)
