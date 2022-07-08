
add_library(se STATIC "")

target_sources(se
    PRIVATE
        SecureElement.hxx
        SecureElement.cxx
        SecureElementChannel.hxx
        SecureElementChannel.cxx
)

set_target_properties(se PROPERTIES FOLDER lib/se)

add_subdirectory(globalplatform)
add_subdirectory(iso)
