
add_library(seglobalplatform STATIC "")

target_sources(seglobalplatform
    PRIVATE
        SecurityDomain.hxx
        SecurityDomain.cxx
)

set_target_properties(seglobalplatform PROPERTIES FOLDER lib/se)
