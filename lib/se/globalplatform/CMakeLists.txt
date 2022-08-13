
add_library(seglobalplatform STATIC "")

set(SE_GLOBALPLATFORM_DIR_PUBLIC_INCLUDE_PREFIX ${SE_DIR_PUBLIC_INCLUDE_PREFIX}/globalplatform)

target_sources(seglobalplatform
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SecurityDomain.cxx
    PUBLIC
        ${SE_GLOBALPLATFORM_DIR_PUBLIC_INCLUDE_PREFIX}/SecurityDomain.hxx
)

target_link_libraries(seglobalplatform
    PUBLIC
        se
)

set_target_properties(seglobalplatform PROPERTIES FOLDER lib/se)
