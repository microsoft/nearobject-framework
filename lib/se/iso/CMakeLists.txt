
add_library(seiso STATIC "")

target_sources(seiso
    PRIVATE
        Apdu.hxx
        Apdu.cxx
)

set_target_properties(seiso PROPERTIES FOLDER lib/se)
