
add_executable(nearobject-test-windows)

target_sources(nearobject-test-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceDiscoveryAgentUwb.cxx 
        ${CMAKE_CURRENT_LIST_DIR}/TestNotStd.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbCxAdapterDdiLrpConversion.cxx 
)

target_link_libraries(nearobject-test-windows
    PRIVATE
        Catch2::Catch2WithMain
        magic_enum::magic_enum
        nearobject-service-windows
        notstd-windows
        uwb
        windows-devices-uwb
        windows-devices-uwb-cx-adapter
)

set_target_properties(nearobject-test-windows PROPERTIES FOLDER test/unit)

catch_discover_tests(nearobject-test-windows)
