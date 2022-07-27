project(no_tlv)

add_library(no_tlv STATIC "")

target_sources(no_tlv
    PRIVATE
        Tlv.cxx
        TlvBer.cxx
)

target_include_directories(no_tlv
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(no_tlv
    PUBLIC
        GSL
)

set_target_properties(no_tlv PROPERTIES FOLDER lib/shared/tlv)
