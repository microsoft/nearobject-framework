
add_executable(notstd-test)

target_sources(notstd-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdTaskQueue.cxx
)

target_link_libraries(notstd-test
    PRIVATE
        Catch2::Catch2
        notstd
)

catch_discover_tests(notstd-test)

