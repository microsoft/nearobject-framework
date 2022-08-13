
add_library(seglobalplatform STATIC "")

target_sources(seglobalplatform
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SecurityDomain.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/SecurityDomain.hxx
)

set_target_properties(seglobalplatform PROPERTIES FOLDER lib/se)
