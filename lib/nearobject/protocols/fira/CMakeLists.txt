add_library(nearobject-proto-fira)

target_sources(nearobject-proto-fira
    PRIVATE
        FiraDevice.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/FiraDevice.hxx
)

target_link_libraries(nearobject-proto-fira
    PRIVATE
        nearobject-proto-fira-uci
)

set_target_properties(nearobject-proto-fira PROPERTIES FOLDER lib/nearobject/protocol/fira)

add_subdirectory(uci)
