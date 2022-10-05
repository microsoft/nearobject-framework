
add_executable(nearobject-test-windows)

target_sources(nearobject-test-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceDiscoveryAgentUwb.cxx 
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStd.cxx
)

target_link_libraries(nearobject-test-windows
    PRIVATE
        Catch2::Catch2WithMain
        nearobject-service-windows
        notstd-windows
)

catch_discover_tests(nearobject-test-windows)
