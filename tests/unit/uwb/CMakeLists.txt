
add_executable(uwb-test)

target_sources(uwb-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbMacAddress.cxx
)

target_link_libraries(uwb-test
    PRIVATE
        Catch2::Catch2
        uwb
)

catch_discover_tests(uwb-test)
