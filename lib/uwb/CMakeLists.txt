
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        IUwbDevice.hxx
        UwbDevice.hxx
        UwbDevice.cxx
)

target_include_directories(uwb
    PRIVATE
        uci
)

target_link_libraries(uwb
    PRIVATE
        uwbuci
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)

add_subdirectory(uci)
