
add_library(notstd INTERFACE)

target_sources(notstd
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/notstd/memory.hxx
)

target_include_directories(notstd
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)

set_target_properties(notstd PROPERTIES FOLDER lib/shared/notstd)
