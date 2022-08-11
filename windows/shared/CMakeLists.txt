
add_library(notstd-windows INTERFACE)

target_sources(notstd-windows
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/notstd/guid.hxx
)

target_include_directories(notstd-windows
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(notstd-windows
    INTERFACE
        rpcrt4.lib
)

set_target_properties(notstd-windows PROPERTIES FOLDER windows/shared/notstd)
