
add_executable(nearobject-test-windows)

target_sources(nearobject-test-windows
    PRIVATE
        Main.cxx
        TestNearObjectDeviceDiscoveryAgentUwb.cxx 
        TestNotStd.cxx
)

target_link_libraries(nearobject-test-windows
    PRIVATE
        Catch2::Catch2
        nearobject-service-windows
        notstd-windows
)

catch_discover_tests(nearobject-test-windows)
