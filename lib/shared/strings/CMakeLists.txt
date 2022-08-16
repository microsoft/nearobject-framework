
add_library(strings INTERFACE)

target_sources(strings
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/strings/tostring.hxx
)

target_include_directories(strings
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)

