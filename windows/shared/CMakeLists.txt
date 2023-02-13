add_subdirectory(logging)

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
        WIL::WIL
)

list(APPEND NOTSTDWINDOWS_PUBLIC_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/notstd/guid.hxx
)

set_target_properties(notstd-windows PROPERTIES FOLDER windows/shared/notstd)
set_target_properties(notstd-windows PROPERTIES PUBLIC_HEADER "${NOTSTDWINDOWS_PUBLIC_HEADERS}")

install(
    TARGETS notstd-windows
    EXPORT notstd-windows
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/notstd
)
