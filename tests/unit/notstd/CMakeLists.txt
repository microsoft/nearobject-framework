
add_executable(notstd-test)

target_sources(notstd-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdFlextypeWrapper.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdHash.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdRange.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdScopeExit.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdTaskQueue.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStdUtility.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUniquePtrOut.cxx
)

target_link_libraries(notstd-test
    PRIVATE
        Catch2::Catch2WithMain
        notstd
)

set_target_properties(notstd-test PROPERTIES FOLDER test/unit)

catch_discover_tests(notstd-test)
