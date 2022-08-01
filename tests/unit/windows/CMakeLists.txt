
add_executable(nearobject-test-windows)

target_link_libraries(nearobject-test-windows
    PRIVATE
        Catch2::Catch2
        nearobject
)

target_include_directories(nearobject-test-windows
    PRIVATE
        ${NOF_DIR_LIB}
)

target_sources(nearobject-test-windows
    PRIVATE
        Main.cxx
)

catch_discover_tests(nearobject-test-windows)
