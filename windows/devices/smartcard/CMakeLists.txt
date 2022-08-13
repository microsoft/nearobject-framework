
add_library(windevsmartcard STATIC "")

target_sources(windevsmartcard
    PRIVATE
        Smartcard.cxx
        Smartcard.hxx
)

target_link_libraries(windevsmartcard
    PRIVATE
        smartcard
)

set_target_properties(windevsmartcard PROPERTIES FOLDER windows/devices)
