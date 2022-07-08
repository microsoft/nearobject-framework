
add_library(windevsmartcard STATIC "")

target_sources(windevsmartcard
    PRIVATE
        SmartCard.cxx
        SmartCard.hxx
)

target_include_directories(windevsmartcard
    PRIVATE
        ${UWB_DIR_LIB}
)

target_link_libraries(windevsmartcard
    PRIVATE
        smartcard
)

set_target_properties(windevsmartcard PROPERTIES FOLDER windows/devices)
